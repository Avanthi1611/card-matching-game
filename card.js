let globalLock = false;
let locked = new Array(16).fill(false);
let flippedCards = [];
let matchedCards = 0;

let cardImages = [
  'assets/card1.png', 'assets/card1_pair.png',
  'assets/card2.png', 'assets/card2_pair.png',
  'assets/card3.png', 'assets/card3_pair.png',
  'assets/card4.png', 'assets/card4_pair.png',
  'assets/card5.png', 'assets/card5_pair.png',
  'assets/card6.png', 'assets/card6_pair.png',
  'assets/card7.png', 'assets/card7_pair.png',
  'assets/card8.png', 'assets/card8_pair.png'
];

function shuffle(array) {
  console.log("Shuffling cards...");
  for (let i = array.length - 1; i > 0; i--) {
    const j = Math.floor(Math.random() * (i + 1));
    [array[i], array[j]] = [array[j], array[i]];
  }
  console.log("Shuffled cardImages:", array);
  return array;
}

function initGame() {
  console.log("Initializing game...");
  const gameBoard = document.getElementById("game-board");
  gameBoard.innerHTML = "";
  cardImages = shuffle(cardImages);

  // Pass shuffled cards to C backend
  cardImages.forEach((card, i) => {
    Module.ccall("set_card", null, ["number", "string"], [i, card]);
  });

  for (let i = 0; i < 16; i++) {
    const card = document.createElement("div");
    card.classList.add("card");
    card.id = `card-${i}`;

    const cardInner = document.createElement("div");
    cardInner.classList.add("card-inner");

    const front = document.createElement("div");
    front.classList.add("card-front");
    front.innerHTML = `<img src="${cardImages[i]}" alt="Card Front" class="card-image">`;

    const back = document.createElement("div");
    back.classList.add("card-back");
    back.innerHTML = `<img src="assets/back_card.png" alt="Card Back" class="card-image">`;

    cardInner.appendChild(back);
    cardInner.appendChild(front);
    card.appendChild(cardInner);
    card.onclick = () => {
      console.log(`Card ${i} clicked`);
      flipCard(i);
    };
    gameBoard.appendChild(card);
  }

  Module._init_game();
  console.log("Game initialized");
}

function flipCard(index) {
  console.log(`flipCard() called with index: ${index}`);
  if (flippedCards.includes(index) || globalLock || locked[index]) {
    console.log("Flip ignored: already flipped or locked");
    return;
  }

  // If the card is part of the hint, remove the glow before flipping
  if (document.getElementById(`card-${index}`).classList.contains("hint-glow")) {
    console.log(`Flipping hinted card: ${index}`);
    document.getElementById(`card-${index}`).classList.remove("hint-glow"); // Remove the hint glow
  }

  // Flip the card
  Module._flip_card(index);
  flippedCards.push(index);
  flipCardDOM(index);
  console.log("Flipped Cards:", flippedCards);

  // Check match if two cards are flipped
  if (flippedCards.length === 2) {
    locked = true; // Lock the game to prevent further flips
    let [index1, index2] = flippedCards;
    console.log(`Checking match between ${index1} and ${index2}...`);
    let isMatch = Module._check_match(index1, index2);
    console.log(`Match result: ${isMatch}`);

    // Handle match result
    if (isMatch) {
      matchedCards++;
      console.log(`Matched Cards: ${index1} and ${index2}`);
      console.log(`Total matched pairs: ${matchedCards}`);
      
      // Hide matched cards after short delay
      setTimeout(() => {
        document.getElementById(`card-${index1}`).style.visibility = "hidden";
        document.getElementById(`card-${index2}`).style.visibility = "hidden";
        
        if (matchedCards === 8) {
          console.log("All cards matched! Showing win message.");
          showMessage("You won!");
        }
        locked = false; // Unlock after match
      }, 500);
    } else {
      console.log(`No match. Flipping cards back.`);
      
      // Flip cards back after short delay
      setTimeout(() => {
        flipCardDOM(index1);
        flipCardDOM(index2);
        locked = false; // Unlock after no match
      }, 1000);
    }

    flippedCards = []; // Reset flipped cards for next round
  }
}


function flipCardDOM(index) {
  const card = document.getElementById(`card-${index}`);
  
  // If the card is hinted, allow flip even if it's not matched
  if (card.classList.contains("hint-glow")) {
    console.log(`Card ${index} is hinted. Allowing flip.`);
    card.classList.remove("hint-glow"); // Remove hint glow once flipped
  }

  // If card is already matched and not hinted, don't flip
  if (Module._is_matched(index) && !card.classList.contains("hint-glow")) {
    console.log(`Card ${index} is already matched. No DOM flip.`);
    return;
  }

  // Toggle card flip normally
  card.classList.toggle("flipped");
  console.log(`Toggled flip for card-${index}`);
}

function undoMove() {
  console.log("Undo move triggered");
  if (flippedCards.length > 0) {
    const last = flippedCards.pop();
    Module._undo_flip();
    flipCardDOM(last);
    console.log(`Undid flip for card-${last}`);
  } else {
    Module._undo_flip();
    for (let i = 0; i < 16; i++) {
      const card = document.getElementById(`card-${i}`);
      if (!Module._is_matched(i)) {
        card.classList.remove("flipped");
      }
    }
    console.log("Reset all unmatched cards");
  }
}

function restartGame() {
  console.log("Restarting game...");
  matchedCards = 0;
  flippedCards = [];
  Module._init_game();

  const cards = document.querySelectorAll(".card");
  cards.forEach(card => card.classList.remove("flipped"));
  showMessage("Game restarted!");
  initGame();
}

function showMessage(message) {
  console.log("Show Message:", message);
  const messageElement = document.getElementById("message");
  messageElement.textContent = message;
}

function showHint() {
  console.log("Entering get_hint function");
  Module.ccall('get_hint', null, [], []);
  const hintPtr = Module._get_hint_pair();
  const hint1 = Module.HEAP32[hintPtr >> 2];
  const hint2 = Module.HEAP32[(hintPtr >> 2) + 1];

  if (hint1 >= 0 && hint2 >= 0) {
    highlightHint(hint1);
    highlightHint(hint2);
    console.log(`Found hint pair: (${hint1}, ${hint2})`);
    locked[hint1] = false;
    locked[hint2] = false;
  } else {
    showMessage("No more hints or matches available.");
  }
  updateHintButton();
  globalLock = false;
}

function highlightHint(index) {
  const card = document.getElementById(`card-${index}`);
  if (card) {
    card.classList.add("hint-glow");
  } else {
    console.warn(`highlightHint: Card element not found for index ${index}`);
  }
}

function updateHintButton() {
  const hintCount = Module.ccall('get_hint_count', 'number', [], []);
  const hintButton = document.getElementById('hint-button');
  hintButton.textContent = `Hint(${3 - hintCount}/3)`;
  if (hintCount >= 3) {
    document.getElementById("hint-button").disabled = true;
  }
}