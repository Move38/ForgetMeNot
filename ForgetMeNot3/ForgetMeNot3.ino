
enum puzzleType_t {
  COLOR,        // Each petal is a single color
  DIRECTION,    // Each petal is lit a single direction
  DICHROMIC,    // Each petal is a pair of colors
  ROTATION      // Each petal animates rotation CW or CCW
};


// Info about a level

struct level_t {
  puzzleType_t type:2;
  byte difficulty:3;    // Higher difficulty = smaller roation angles
  byte changes:3;       // Number of tiles that change in this round
};

#define MAX_LEVEL 72

// Make PROGMEM if we run out of RAM
/*
 * Once the levels get higher up, we should have a sort of random sequence of puzzle types
 * What could be our source of inspiration for this randomness?!
 * How about: https://phys.org/news/2018-12-forget-me-not-scientists-memory-mechanism.html
 * Let's use the genetic sequence and give each one of the puzzle types an A, C, G, T affiliation
 * Can you find the portion of this games sequence that exists somewhere else in nature :)
 * Here is a starting point https://www.ncbi.nlm.nih.gov/gene/
 */
level_t levels[MAX_LEVEL] {
  { COLOR     , 1 , 1  }, // Level  0
  { COLOR     , 1 , 1  }, // Level  1
  { COLOR     , 1 , 1  }, // Level  2
  { COLOR     , 1 , 1  }, // Level  3
  { COLOR     , 2 , 1  }, // Level  4
  { DIRECTION , 1 , 1  }, // Level  5
  { DIRECTION , 1 , 1  }, // Level  6
  { COLOR     , 2 , 1  }, // Level  7
  { DIRECTION , 1 , 1  }, // Level  8
  { DIRECTION , 2 , 1  }, // Level  9
  { DICHROMIC , 1 , 1  }, // Level  10
  { DICHROMIC , 1 , 1  }, // Level  11
  { DIRECTION , 1 , 1  }, // Level  12
  { COLOR     , 2 , 1  }, // Level  13
  { DICHROMIC , 2 , 1  }, // Level  14
  { ROTATION  , 1 , 1  }, // Level  15
  { ROTATION  , 1 , 1  }, // Level  16
  { DICHROMIC , 2 , 1  }, // Level  17
  { COLOR     , 3 , 1  }, // Level  18
  { ROTATION  , 1 , 1  }, // Level  19
  { COLOR     , 2 , 1  }, // Level  20
  { DIRECTION , 2 , 1  }, // Level  21
  { DICHROMIC , 2 , 1  }, // Level  22
  { ROTATION  , 2 , 1  }, // Level  23
  { COLOR     , 2 , 1  }, // Level  24
  { DIRECTION , 2 , 1  }, // Level  25
  { DIRECTION , 2 , 1  }, // Level  26
  { ROTATION  , 2 , 1  }, // Level  27
  { COLOR     , 2 , 1  }, // Level  28
  { DIRECTION , 2 , 1  }, // Level  29
  { DIRECTION , 2 , 1  }, // Level  30
  { ROTATION  , 2 , 1  }, // Level  31
  { ROTATION  , 2 , 1  }, // Level  32
  { DIRECTION , 3 , 1  }, // Level  33
  { COLOR     , 3 , 1  }, // Level  34
  { COLOR     , 3 , 1  }, // Level  35
  { DICHROMIC , 3 , 1  }, // Level  36
  { ROTATION  , 3 , 1  }, // Level  37
  { ROTATION  , 3 , 1  }, // Level  38
  { DIRECTION , 3 , 1  }, // Level  39
  { ROTATION  , 3 , 1  }, // Level  40
  { DIRECTION , 3 , 1  }, // Level  41
  { ROTATION  , 3 , 1  }, // Level  42
  { COLOR     , 3 , 1  }, // Level  43
  { DICHROMIC , 3 , 1  }, // Level  44
  { DIRECTION , 3 , 1  }, // Level  45
  { ROTATION  , 4 , 1  }, // Level  46
  { COLOR     , 4 , 1  }, // Level  47
  { DIRECTION , 4 , 1  }, // Level  48
  { DICHROMIC , 4 , 1  }, // Level  49
  { DICHROMIC , 4 , 1  }, // Level  50
  { COLOR     , 4 , 1  }, // Level  51
  { ROTATION  , 4 , 1  }, // Level  52
  { DIRECTION , 4 , 1  }, // Level  53
  { COLOR     , 4 , 1  }, // Level  54
  { ROTATION  , 4 , 1  }, // Level  55
  { DIRECTION , 4 , 1  }, // Level  56
  { COLOR     , 4 , 1  }, // Level  57
  { DICHROMIC , 4 , 1  }, // Level  58
  { COLOR     , 4 , 1  }, // Level  59
  { COLOR     , 4 , 1  }, // Level  60
  { DIRECTION , 4 , 1  }, // Level  61
  { DIRECTION , 4 , 1  }, // Level  62
  { ROTATION  , 4 , 1  }, // Level  63
  { ROTATION  , 4 , 1  }, // Level  64
  { ROTATION  , 4 , 1  }, // Level  65
  { DIRECTION , 4 , 1  }, // Level  66
  { ROTATION  , 4 , 1  }, // Level  67
  { COLOR     , 4 , 1  }, // Level  68
  { DIRECTION , 4 , 1  }, // Level  69
  { DICHROMIC , 4 , 1  }, // Level  70
  { ROTATION  , 4 , 1  }, // Level  71
};


// Are we currently the center?
bool weAreCenter=false;

// The current game state, only valid when weAreCenterFlag is true

enum gameState_t {

  RESET,                // Get ready to start a new game as center.
                        // Exit when both (1) all petals are ready and (2) button is lifted (makes for nice smooth UI)

  BLOOM,                // Show that we are getting ready to start next level
                        // Center animate green to yellow, petals animate full green to just one green pixel facing center
                        // Exit on button press on center

  PAUSE,                // Game paused. Happens when one or more petals are removed.

  PUZZLE,               // Show the puzzle
                        // Exit at end of timeout

  HIDE,                 // Pause to make them forget what they saw
                        // Exit at end of timeout

  CHANGED,              // Show the puzzle but changed from the last PUZZLE state
                        // Exit when user pushes one of the petals

  // Jump to ANSWER if user selects wrong tile

  CORRECT,              // User found the change.
                        // Exit at end of timeout.


  // Jump to BLOOM if there is another level
  // Jump to WIN if no more levels

  ANSWER,          // User picked wrong change, show them the right one
                        // All petals red except the one that changed which is full green.
                        // Center stays yellow
                        // Exit at end of timeout and go to scoreboard.

  SCOREBOARD,           // Show what level they got to with a full screen animation
                        // Exit on user longpress center to start new game

  // Jump to SETUP


  WIN,                  // They beat all the levels!
                        // Exit on user triple clicks center to start new game


};


// A long press anytime jumps to BLOOM if this tile has 6 neighbors
// Anytime a leaf goes too long without seeing any messages, it will go back to SETUP
// Anytime a center sees a RESET, it will become a leaf

// Messages sent over the IR link

enum messages_t {

  // Sent by Center

  SHOW_RESET,         // Show single yellow pixel pointing to center. Reset level to 0.
  SHOW_BLOOM,         // Show single green pixel pointing to center. Increment level.
  SHOW_PAUSE,         // Show full green. Also shown on a petal with no center. There is an anmiation transition from this to SHOW_BLOOM.

  SHOW_PUZZLE,        // Pick a random puzzle to show based on puzzle type for current level.
  SHOW_HIDE,          // Show green on face pointing to center
  SHOW_UNCHANGED,     // Identical to SHOW_PUZZLE except will send a button if pressed. Broken out for clarity and ease of adding stuff later.
  SHOW_CHANGED,       // Change your display based on current puzzle type and difficulty (only sent to faces that should change)
  SHOW_CORRECT,       // When user makes correct pick, all tiles get this (currently green).
  SHOW_WRONG_MISSED,  // When user makes a wrong pick, this is shown on the missed tile (currently green)
  SHOW_WRONG_OTHERS,  // When user makes a wrong pick, this is shown on all but the missed tile (currently red)

  SHOW_WIN,           // When user beats all levels (currently animated rainbow)

  SHOW_SCORE_0,       // Score beofre the indicator has come around to it. Currently the same visually as SHOW_BLOOM
  SHOW_SCORE_1,       // 1-3 edge LEDs lit
  SHOW_SCORE_2,
  SHOW_SCORE_3,

  // Sent by Petals

  SIGNAL_READY,                 // Ready to start a new game
  SIGNAL_IDLE,                  // Sent by default
  SIGNAL_PRESSED,               // Button pressed since last transition. Cleared whenever the petal gets a new message from center.

};


// Current play level
byte currentLevel;

const byte GREEN_HUE = 77;
const byte YELLOW_HUE = 42;


// Cycle though these colors on each rotation around the scoreboard. Taken from the instruction manual.
Color scoreboard_cycle_colors[] = { OFF , RED , makeColorRGB(255,160,0) , YELLOW, GREEN };

struct stateTimer_t {

  Timer timer;

  // We remember the durration of the current timer so we can calculate our progress into it.
  // (There really should be a Timer.getProgress() )
  word initialDurration;

  void set( word t ) {
    timer.set( t );
    initialDurration = t;
  }

  // Returns 0-255 to indicate how far along the timer we are (255=expired)
  byte progress() {

    if (timer.isExpired()) {
      return 255;
    }

    return 255-(( timer.getRemaining() * 255UL ) / initialDurration);

  }

};

stateTimer_t stateTimer;


// How long to show the bloom animation at the start of each round
const word BLOOM_TIME_MS=1200;

// How long to show the "you are correct" display after a correct changed petal selection
const word CORRECT_TIME_MS=1800;

// How long to display which petal they should have picked when they pick wrong.
const word ANSWER_TIME_MS = 3000;

// How long between when we drop into scoreboard display and when we start ticking off levels
const word SCORE_START_TIME_MS = 500;

// How long between level ticks in scoreboard display.
const word SCORE_TICK_TIME_MS = 100;

void setup() {
  // Default display at power up
  // TODO: Do we want to also start here after waking from sleep?
  setColor( GREEN );
  setValueSentOnAllFaces( SIGNAL_IDLE );
}


// Control puzzle show and hide times based on level
// TODO: TUNE THESE
const word MAX_SHOW_TIME=5000;    // 5 seconds on
const word MIN_SHOW_TIME=3000;    // 3 seconds on
const word MIN_HIDE_TIME=1500;    // 1.5 second  off
const word MAX_HIDE_TIME=4000;    // 4 seconds off
const byte CURVE_BEGIN_LEVEL=10;  // at this level start adjusting the on and off time
const byte CURVE_END_LEVEL=45;    // at this level stop adjusting the on and off time (as hard as the timing gets)

/*
   returns a duration in milliseconds that the puzzle will show for
*/
word getShowDuration(byte level) {
  if (level < CURVE_BEGIN_LEVEL) {
    return MAX_SHOW_TIME; // max time is easier i.e. lower level
  }
  else if (level > CURVE_END_LEVEL) {
    return MIN_SHOW_TIME; // min time is harder i.e. higher level
  }
  else {
    // everything in between
    return MAX_SHOW_TIME - (level - CURVE_BEGIN_LEVEL) * (MAX_SHOW_TIME - MIN_SHOW_TIME) / ( CURVE_END_LEVEL - CURVE_BEGIN_LEVEL);
  }

}

/*
   returns a duration in milliseconds that the puzzle will show for
*/
word getHideDuration(byte level) {

  if (level < CURVE_BEGIN_LEVEL) {
    return MIN_HIDE_TIME; // max time is easier i.e. lower level
  }
  else if (level > CURVE_END_LEVEL) {
    return MAX_HIDE_TIME; // min time is harder i.e. higher level
  }
  else {
    // everything in between
    return MIN_HIDE_TIME + (level - CURVE_BEGIN_LEVEL) * (MAX_HIDE_TIME - MIN_HIDE_TIME) / ( CURVE_END_LEVEL - CURVE_BEGIN_LEVEL);
  }
}

boolean doWeHave6Neighboors() {
  FOREACH_FACE(f) {
    if (isValueReceivedOnFaceExpired(f)) {
      return false;
    }
  }
  return true;
}


void showWinAnimation() {

    setColor( GREEN );
    // They got it right, I think they deserve little excitment with some fading sparkles
    setColorOnFace( WHITE , random( FACE_COUNT -1 ) );
    //uint32_t m = millis();
    //setColor( makeColorHSB( ( m >> 6 ) & 0xff , 0xff , m & 0xff ) );     // TODO: After 72 levels, we owe them better than this crappy rainbow.
}

// Current game state (only valid if weAreCenterFlag==true)
gameState_t gameState;

// Which petal changed? (only valid in CHANGED state)
byte changedPetal;

// Used by the center for the final scoreboard animatimation
byte scoreboard_tick_face=0;
byte scoreboard_tick_step=0;
byte scoreboard_tick_cycle=0;

// Note when we start the scoreboard to animate correctly
uint32_t timeOfStartScoreBoard;
byte maxLevelReached;

// We are the center, update our state

void updateStateCenter() {

  byte progress = stateTimer.progress();      // Used in several places, so grab once here in case we need it.

  switch (gameState) {

    case RESET: {

      bool allPetalsReadyFlag = true;

      FOREACH_FACE(f) {
         if ( !isValueReceivedOnFaceExpired(f) && getLastValueReceivedOnFace(f) == SIGNAL_READY ) {
          setColorOnFace( GREEN , f );
         } else {
          allPetalsReadyFlag = false;
         }
      }

      if (allPetalsReadyFlag) {
        gameState=BLOOM;
        stateTimer.set( BLOOM_TIME_MS );
        setValueSentOnAllFaces( SHOW_BLOOM );    // Get petals ready for next round.
      }

    } return;

    case BLOOM: {

      // Durring bloom animation we change color from GREEN to YELLOW. Ends with YELLOW after timer expires.
      byte hue = ( ( YELLOW_HUE * progress ) + ( GREEN_HUE * (255-progress)) ) / 255;
      setColor( makeColorHSB(  hue , 255 , 255 ) );

      // Here we implement "pause" mode. If the user removes one of the 6 petals durring bloom,
      // the cneter displays just yellow (no sparkle) and the user can not start the round with
      // a button press until the 6th petal is replaced

      if ( doWeHave6Neighboors() ) {

       if (buttonPressed()) {
          // Pressing the button now will start the round and show the puzzle on the petals.
          // Note that the petals decide what to show since they know what level we are on
          setColor(YELLOW);   // Clear the sparkle
          setValueSentOnAllFaces( SHOW_PUZZLE );

          gameState=PUZZLE;
          stateTimer.set( getShowDuration(currentLevel) );
        } else {

          // increase the sparkle over time
          setColorOnFace( makeColorHSB(hue, 255-progress, 255), random(FACE_COUNT-1) );

        }

      } else {
        // We are paused, so consume any button press so it does not linger until the end of the pause
        buttonPressed();
      }


    } return;


    case PUZZLE: {
      // During this time, the user gets to study the puzzle.
      // A little countdown stopwatch animation here for fun.

      if (progress < 255 ) {
        // Animation running, show center face ticking down to full black...
        byte darkface = (byte) ((progress * (FACE_COUNT)) / 255);   // Intiontionally 1 too big so we get an extra  step with all pixels on (looks nicer)
        if (darkface>0) {
          setColorOnFace( OFF , darkface-1 );
        }
      } else {
        // Your time to study the puzzle is up! Go dark!
        setColor(OFF);
        setValueSentOnAllFaces( SHOW_HIDE );
        gameState=HIDE;
        stateTimer.set( getHideDuration(currentLevel) );
      }
    } return;


    case HIDE: {
      // All tiles are OFF right now from the end of PUZZLE.

      if (progress==255) {   // Done with HIDE?

        // Only now, at the very last second do we actually pick which petal changed. Seems morally wrong, right?
        changedPetal = random( FACE_COUNT- 1) ;

        // Have the petals display either the orginal or changed version of thier puzzle
        FOREACH_FACE(f) {
          if (f==changedPetal) {                        // TODO: Condense with `x?y:z` if we need space
            setValueSentOnFace( SHOW_CHANGED , f );
          } else {
            setValueSentOnFace( SHOW_UNCHANGED , f );
          }
        }

        setColor(YELLOW);
        gameState=CHANGED;
      }
    } return;

   case CHANGED: {
      // User is looking at the puzzle with a single petal changed.
      // This state ends with the press one of the tiles. There is no timeout.

      // scan though all petals and see if any buttons have been pressed
      FOREACH_FACE(f) {
        if (!isValueReceivedOnFaceExpired(f) && getLastValueReceivedOnFace(f)==SIGNAL_PRESSED ) {
          // This petal had its button pressed
          if (f==changedPetal) {
            // They picked the right petal!
            // TODO: Make even harder stages where you have to pick mulitple changed petals! (already in the level data structure)
            gameState = CORRECT;
            setValueSentOnAllFaces( SHOW_CORRECT );
            stateTimer.set( CORRECT_TIME_MS );
            setColor(GREEN);
          } else {
            // They pressed the wrong petal :(
            setValueSentOnAllFaces(  SHOW_WRONG_OTHERS );
            setValueSentOnFace( SHOW_WRONG_MISSED , changedPetal );     // Show them the one they _should_ have pressed.
            gameState=ANSWER;
            stateTimer.set( ANSWER_TIME_MS );
            setColor(RED);
          }
        }
      }
    } return;

    case CORRECT: {

      // They got it right, user is watching a little "horray" animation on the petals with solid green in the center

      if (progress == 255) {
        // Ready for next round.
        currentLevel+=1;

        if (currentLevel==MAX_LEVEL) {

          // WE HAVE A WINNER!

          gameState=WIN;
          setValueSentOnAllFaces( SHOW_WIN );    // A spectacular flurish

        } else {
          // Next round
          gameState=BLOOM;
          stateTimer.set( BLOOM_TIME_MS );
          setValueSentOnAllFaces( SHOW_BLOOM );    // Get petals ready for next round.
        }
      }

    } return;

    case ANSWER: {
      // Wait for them to see the answer they should have picked.

      if (progress==255) {
        gameState = SCOREBOARD;
        timeOfStartScoreBoard = millis();
        maxLevelReached = currentLevel;
        scoreboard_tick_face=0;
        scoreboard_tick_step=0;
        scoreboard_tick_cycle=1;
        setColor( scoreboard_cycle_colors[scoreboard_tick_cycle] ); // fill red from the start
        setValueSentOnAllFaces( SHOW_SCORE_0 );
        stateTimer.set( SCORE_START_TIME_MS );
      }

    } return;


    case SCOREBOARD: {

      if (progress==255) {      // Same result, but more efficient that checking isExpired()
        // time for next tick

        if (currentLevel) {
          // Still have more levels to tick off
          currentLevel--;

          // Convert the step into a sendable message. There should be a better way.
          static const byte scoreboard_tick_messages[] = { SHOW_SCORE_1 , SHOW_SCORE_2 , SHOW_SCORE_3 };

          setValueSentOnFace( scoreboard_tick_messages[scoreboard_tick_step] , scoreboard_tick_face );

          // I think this helps make the score spin animation look nicer?

          //setColorOnFace( scoreboard_cycle_colors[scoreboard_tick_cycle] , scoreboard_tick_face );
          setColor(scoreboard_cycle_colors[scoreboard_tick_cycle]);

          scoreboard_tick_step++;

          if (scoreboard_tick_step==3) {
            scoreboard_tick_step=0;
            scoreboard_tick_face++;

            if (scoreboard_tick_face == FACE_COUNT ) {
              // TODO: prevent users from making fake scores... note, we can tell because the center shows the true level achieved
              scoreboard_tick_face = 0;   // Wrap around. Petals will keep track of how many times they have been ticked and will take care of cycling to the next color.
              scoreboard_tick_cycle++;    // Update the color we use for spinning the center.
            }
          }

          stateTimer.set( SCORE_TICK_TIME_MS );
          buttonPressed();    // Clear any pending button presses so we can detect the press when the piece returns

        } else {
          // ..and this this lets you know the score is done spinning out
          // TODO: Animate the current round color fading up and down
          uint32_t offsetTimePassed = millis() - ( timeOfStartScoreBoard + (maxLevelReached * SCORE_TICK_TIME_MS));
          setColor(dim(scoreboard_cycle_colors[scoreboard_tick_cycle], sin8_C(millis()/8)));

          // listen for an reset with a single button pressed
          if(buttonPressed()) {
            resetGameBecomeCenter();
          }
        }
      }

    } return;

    case WIN: {
      // They deserve a nice rainbox.
      // Exits when user triple clicks a center to start new game

      showWinAnimation();
    } return;
  }
}

#define PETAL_COLOR_COUNT 6

Color petalColors[] = {
  makeColorHSB( 30,200,255),  // SALMON
  makeColorHSB(255,200,255),  // PINK
  makeColorHSB(220,200,255),  // LIGHT PINK
  makeColorHSB(180,200,255),  // VIOLET
  makeColorHSB(150,200,255),  // INDIGO
  makeColorHSB(120, 50,255),  // PERIWINKLE/WHITE
  BLUE
};

byte getRandomPetalColorIndex() {
  return random( PETAL_COLOR_COUNT -1 );
}

// This find a different color, and how hard it is to see it is different is determined by the difficulty
  /*
     NOTE: difficulty distance should be thought of from the midpoint
     i.e. the easiest difference in color will be 180º away in the spectrum
     and the easiest difference in direction will be 180º opposite
  */

byte getContraColorIndex( const byte startColorIndex , const byte difficulty ) {

  byte endColorIndex;

  bool goRight = (random(1)==0);      // Go right around the color wheel (clockwise)?

  // TODO: We should just code this distance directly into the levels struct?

  byte distance = 5 - difficulty;

  // TODO: Am I reading this right? Seems like the values for `difficulty` in levels should start at 3 and then go down to 1 as levels go up? 0 and 4 really don't make sense.
  // TODO: Yea, a difficulty of 1 now turns into a distance of 4, which is harder than a difficulty of 2 which is a distance of 3.

  if (goRight) {
    endColorIndex = startColorIndex + distance;
  } else {
    endColorIndex = startColorIndex + (PETAL_COLOR_COUNT - distance);
  }

  // Normalize the color back into range
  if (endColorIndex>=PETAL_COLOR_COUNT) {
    endColorIndex-=PETAL_COLOR_COUNT;
  }


  return endColorIndex;

}

// Bring f down to 0<=f<FACE_COUNT
// Assumes that f<((FACE_COUNT*2)-1)

byte normalizeFace( byte f ) {
  if (f>=FACE_COUNT) {
    f-=FACE_COUNT;
  }
  return f;
}


byte nextFaceClockwise( byte f ) {
  return normalizeFace( f+1 );
}

// Find opostie face
byte opositeFace( byte f ) {
  return normalizeFace( f + (FACE_COUNT/2) );
}

// Set the 3 faces pointing towards directionFace to c

void setColorIndexOnSide( byte colorIndex , byte directionFace ) {
    Color c =  petalColors[colorIndex ];
    setColorOnFace( c , directionFace );
    setColorOnFace( c , normalizeFace( (directionFace+FACE_COUNT)-1 ) );
    setColorOnFace( c , normalizeFace( directionFace+1 ) );
}

void swap( byte &a , byte &b ) {
  byte t=a;
  a=b;
  b=t;
}

const byte ROTATION_MS_PER_STEP = 50;   // 50ms per step * 6 steps per rotation = 300ms per rotation = ~3 rotations per second
byte rotationBri[6] = {0, 0, 0, 0, 0, 0};
byte rotationFace = 0;
Timer rotationTimer;

struct puzzle_t {

  puzzleType_t type;   // Set based on the current level
  byte difficulty;           // Set based on the current level

  // I know this union seems like overkill, but it makes this complicated code read simple
  // Hey, I could have made a polymorphic puzzle class!

  union {                // We use this to remember the data used to make the current puzzle (and it's changed version)

    struct {
      byte index;         // Index of the current color in petalColors
      byte contraIndex;   // Index of the alternate color to show as changed. This has some randomness, so we we calculate at set time
    } color;

    struct {
      byte interiorIndex;    // Index of the current color in petalColors that points to the center
      byte exteriorIndex;    // Index of the current color in petalColors that points away from center. We could compute this deterministically, but more efficient to remember it once.
    } dicromic;

    struct {
      byte face;              // Which face it lit
    } direction;

    struct {
      boolean clockwise;
    } rotation;

  } data;

  byte currentLevelIndex;     // We remeber the currentl level we are playing to create an interlock for incrementing to the next level

  void set( byte newLevelIndex ) {    // Set the current puzzle

    currentLevelIndex = newLevelIndex;

    const level_t &level = levels[newLevelIndex];

    // Save these, we might need them again
    type = level.type;
    difficulty = level.difficulty;



    switch (type) {

      case COLOR:         // Each petal is a single color
        data.color.index       = getRandomPetalColorIndex();
        data.color.contraIndex = getContraColorIndex( data.color.index  , difficulty );
        break;

      case DIRECTION:     // Each petal is lit a single direction
        data.direction.face = random( FACE_COUNT -1 );
        break;

      case DICHROMIC:     // Each petal is a pair of colors
        data.dicromic.interiorIndex = getRandomPetalColorIndex();
        data.dicromic.exteriorIndex = getContraColorIndex( data.dicromic.interiorIndex , difficulty );
        break;

      case ROTATION:      // Each petal animates rotation CW or CCW
        data.rotation.clockwise = (random(1)==0);
        break;

    }

  }

  void show(byte centerFace, bool changed) {       // Show the puzzle on our pixels. if changed==true, the then alternate version is shown


    switch (type) {

      case COLOR: {         // Show the indicated color index on all pixels

        byte showColorIndex=data.color.index;

        if (changed) {
          showColorIndex =  data.color.contraIndex;
        }

        setColor( petalColors[ showColorIndex ] );
      } break;

      case DIRECTION: {    // Show selected petal as white

        byte showDirectionFace=data.direction.face;

        if (changed) {
          // make the direction point the other way
          // TODO: Should there also be other ways it can pint at harder levels?
          showDirectionFace = opositeFace( showDirectionFace );
        }

        setColor(OFF);
        setColorOnFace( WHITE , showDirectionFace );
      } break;

      case DICHROMIC: {     // Show inner on 3 faces pointing to center, outer on other 3 faces

        byte showFaceDirection = centerFace;
        byte showFaceOtherDirection = opositeFace(showFaceDirection);

        if (changed) {
          // swap inner and outer colors
          swap( showFaceDirection , showFaceOtherDirection );
        }

        setColorIndexOnSide( data.dicromic.interiorIndex , showFaceDirection );
        setColorIndexOnSide( data.dicromic.exteriorIndex , showFaceOtherDirection );
      } break;

      case ROTATION: {  // Show a single white pixel rotating based on millis so they will be out of sync as show in instructions

        // TODO: maybe sync/unsync or varry speed based on difficulty

        // TODO: This will be huge code size, replace if we run out of room.

        if (rotationTimer.isExpired()) {

          rotationTimer.set(ROTATION_MS_PER_STEP);

          if (changed ^ data.rotation.clockwise) { // CW Rotation

            rotationFace = nextFaceClockwise(rotationFace);

          } else {  // CCW Rotation

            if(rotationFace == 0) {
              rotationFace = 5;
            }
            else {
              rotationFace--;
            }
          }

          rotationBri[rotationFace] = 255;
        }

        FOREACH_FACE(f) {
          if ( rotationBri[f] >= 10 ) {
            rotationBri[f] -= 10;
          }
          setColorOnFace(dim(WHITE, rotationBri[f]), f);
        }

      } break;

    }

  }

};


// How many times have we gone around the scoreboard display so far (used to set the color)
byte scoreboard_cycle;

#define MAX_SCOREBOARD_CYCLE 3

// use this to detect when we have going around a full cycle
byte scoreboard_count_prev;

void showScore( byte centerFace , byte count ) {

  if (count ==1 && scoreboard_count_prev==3) {
    // We have gone all the way around, increment to next display color
    if( scoreboard_cycle < MAX_SCOREBOARD_CYCLE ) { // cap at the number of rounds available
      scoreboard_cycle++;
    }
  }

  scoreboard_count_prev = count;

  setColor(OFF);
  setColorOnFace( GREEN , centerFace );

  byte edgeFace = nextFaceClockwise( nextFaceClockwise( centerFace) );  // Start with the first edge pixel which is oposite the center one minus one.

  byte fgColorIndex =  scoreboard_cycle+1;    // Forground color (the cycle we are spinning)
  byte bgColorIndex =  scoreboard_cycle;      // Backgroung color (the previous cycle we are writting over)

  for( byte i =1; i <= 3; i++ ) {

    byte cIndex;

    if (count>=i) {
      cIndex = fgColorIndex;
    } else {
      cIndex = bgColorIndex;
    }

    if(count == i ) {
      setColorOnFace( dim(scoreboard_cycle_colors[cIndex], stateTimer.progress()), edgeFace);
    }
    else {
      setColorOnFace( scoreboard_cycle_colors[cIndex] , edgeFace );
    }

    edgeFace = nextFaceClockwise( edgeFace );

  }

}


puzzle_t puzzle;

// The face of our current center (only valid if areWeCenterFlag==false)
// This defaults to 0, which is OK we will slightly prefer a center on that face if there are more than one center
// which is not really a case we need to care about
byte centerFace;

// The last value we received from the center
// This defaults to 0, which is OK, since reset doesn't have any time based needs
byte messageFromCenter_prev = SHOW_PAUSE;   // Default is to show full green

// Update the petal based on message received from a center on face f
// return true if there was a center on the face, false if nothing or a petal on that face

bool updateStatePetalOnFace(byte f) {

  if (!isValueReceivedOnFaceExpired(f)) {

    byte messageFromCenter = getLastValueReceivedOnFace(f);

    bool messageFromCenterChangeFlag=false;

    if( messageFromCenter != messageFromCenter_prev ) {

      messageFromCenterChangeFlag=true;

      messageFromCenter_prev = messageFromCenter; // only do this once on change

    }


    switch (messageFromCenter) {

      case SHOW_RESET:        // Starts a new game.

        // Note there is logic that is special cased in loop() since it changes global state.

        // The center does not transition from RESET to BLOOM until it sees that all petals are READY, so we set our color
        // to GREEN here seince we don't know how long we might have to wait.
        setColor( GREEN );    // Smooth visual transition into bloom which should come next.

        return true;


      case SHOW_BLOOM:          // Show single green pixel pointing to center. Increment level.

        // Note here that we call puzzle.set() repeatedly while the bloom animation is running. This adds some entorpy.
        puzzle.set( currentLevel );

        if ( messageFromCenterChangeFlag ) {
          // New display, start animation
          stateTimer.set( BLOOM_TIME_MS );
        }


        // Whole tile fades from green to off...
        setColor( dim(GREEN, 255 - stateTimer.progress())  );
        // except pixel pointing towards the center which stays green
        setColorOnFace( GREEN  , f );
        return true;

      case SHOW_PUZZLE:        // Show the current puzzle on our pixels
        puzzle.show( f , false );     // Show a current puzzle, not changed
        return true;

      case SHOW_HIDE:        // The pause between showing the unchanged and changed version
        setColor(OFF);        // This is an editorial decision. I like it all going dark here.

        // Clear any pending button press to get ready for SHOW_CHANGED when user can press button
        // (Clears a SIGNAL_PRESSED if this petal was pressed in last round)
        buttonPressed();
        setValueSentOnAllFaces( SIGNAL_IDLE );

        return true;

      case SHOW_CHANGED:             // Show the current puzzle on our pixels, but in the "changed" version
        puzzle.show( f , true );     // Show a current puzzle, changed

        if (buttonPressed()) {
          setValueSentOnFace( SIGNAL_PRESSED , f  );     // We use the petals's sticky value sent to hold the pressed button state
        }
        return true;

      case SHOW_UNCHANGED:          // Show the current puzzle on our pixels, but in the "changed" version (currently same as shown by SHOW_PUZZLE)
        puzzle.show( f , false );   // Show a current puzzle, unchanged

        if (buttonPressed()) {
          setValueSentOnFace( SIGNAL_PRESSED , f  );     // We use the petals's sticky value sent to hold the pressed button state
        }

        return true;

        // TODO: This could be combined with SHOW_PUZZLE to save space
        // TODO: This could do something to make recognizing the orginal pattern harder at higher difficulty levels.


      case SHOW_CORRECT:             // Indicate to the user they picked the right peice
        // I pick now as a good time to increment the petal to the next level.
        // We check if the currentLevel variable matches the level of the current puzzle to make sure we only
        // increment once per pass though the sequence.
        if (puzzle.currentLevelIndex==currentLevel) {
          currentLevel++;
        }

        if ( messageFromCenterChangeFlag ) {
          // New display, start animation
          stateTimer.set( CORRECT_TIME_MS );
        }

        // They got it right, lets celebrate!
        // Show Green and fade down the outside to only leave the center illuminated
        // TODO: Make this much more efficient
        // This celebration looks pretty nice, could use a little fine tuning, but can also be implemented in a more space efficent way

        {

          // We need a local block here to control scoping of `progress`

          byte progress = stateTimer.progress();

          setColor(dim(GREEN, 255 - progress) );
          setColorOnFace(GREEN,  centerFace );

          // on the exterior, create a rotation animation
          byte globalBri;
          if (progress < 204) {
            globalBri = 255;
          }
          else {
            globalBri = 255 - (5 * ( progress  - 204));
          }
          setColorOnFace( dim(GREEN, (globalBri * sin8_C((9*progress/4)+192))/255), nextFaceClockwise( nextFaceClockwise( centerFace )) );
          setColorOnFace( dim(GREEN, (globalBri * sin8_C((9*progress/4)+128))/255), nextFaceClockwise( nextFaceClockwise( nextFaceClockwise( centerFace ))) );
          setColorOnFace( dim(GREEN, (globalBri * sin8_C((9*progress/4)+64))/255), nextFaceClockwise( nextFaceClockwise( nextFaceClockwise( nextFaceClockwise( centerFace )))) );
        }

        return true;


      case SHOW_WRONG_MISSED:   // Show user they made the wrong choice and this tile was the changed one

        if ( messageFromCenterChangeFlag ) {
          // New display, start animation
          stateTimer.set( ANSWER_TIME_MS );
        }

        setColor( dim(GREEN, 255 - stateTimer.progress()) );
        return true;

      case SHOW_WRONG_OTHERS:   // Show user they made the wrong choice and this tile was the not changed one

        if ( messageFromCenterChangeFlag ) {
          // New display, start animation
          stateTimer.set( ANSWER_TIME_MS );
        }

        setColor( dim(RED, 255 - stateTimer.progress()) );
        return true;

      case SHOW_SCORE_0:
        if ( messageFromCenterChangeFlag ) {
          // New display, start animation
          stateTimer.set( SCORE_TICK_TIME_MS );
        }
        showScore( f , 0 );
        return true;

      case SHOW_SCORE_1:
        if ( messageFromCenterChangeFlag ) {
          // New display, start animation
          stateTimer.set( SCORE_TICK_TIME_MS );
        }
        showScore( f , 1 );
        return true;

      case SHOW_SCORE_2:
        if ( messageFromCenterChangeFlag ) {
          // New display, start animation
          stateTimer.set( SCORE_TICK_TIME_MS );
        }
        showScore( f , 2 );
        return true;

      case SHOW_SCORE_3:
        if ( messageFromCenterChangeFlag ) {
          // New display, start animation
          stateTimer.set( SCORE_TICK_TIME_MS );
        }
        showScore( f , 3 );
        return true;

      case SHOW_WIN:            // Show a pretty winning flurish.
        showWinAnimation();
        return true;

    }
  }

  return false;

}

// Returns true if we have 6 neighboors and all of them are petals
// This is a bit of a hack to cover the case where we want to automatically start
// when 7 tiles come together right after a download/power-up.

bool updateStatePetal() {

  // First check if we are still connected to a center on the expected face

  if (updateStatePetalOnFace(centerFace)) {
    // We found our expected center
    return false;
  }

  // If we get here then we did not find the center on the expected face, so we will look on all the faces.
  // This is slightly redundant (we check one face twice), but that is OK.
  // Note that we only have all this extra logic to handle the edge case of a tile collection getting
  // taken appart in the middle of a game and then getting put back together again but with a petal rotated.
  // This will also result in osscilation in the case where multipule existing centers are put into contact with an existing leaf,
  // but thats OK since you deserve what you get in this case.

  FOREACH_FACE(f) {
    if (updateStatePetalOnFace(f)) {
      // We found a center
      centerFace=f;
      return false;
    }
  }

  // if we get here then we have no center connected at the moment

  // If we do not have a center then show plain green
  setColor( GREEN );
  messageFromCenter_prev = SHOW_PAUSE;    // So we will be fresh when we reconnect
  buttonPressed();    // Clear any pending button presses so we can detect the press when the piece returns

  return false;
}

// This can happen in two cases:
// 1. This is the center of a 7 tile formation and user long presses (manual start)
// 2. This is the center of a 7 tile formation and none of the tiles are currently a center (autostart)

void resetGameBecomeCenter() {
  weAreCenter=true;
  gameState=RESET;
  currentLevel=0;
  setColor(OFF);
  setValueSentOnAllFaces( SHOW_RESET );   // Reset all petals to level 0
  buttonPressed();    // Clear any pending button presses so we can detect the press to start the game from BLOOM state
}

void loop() {

  // Check for resets to the global state machine first

  // First check if any neighboor is starting a new game as center. This will make us his petal
  // unconditionally, even if we are currently a center.

  FOREACH_FACE(f) {
    if ( !isValueReceivedOnFaceExpired(f) && getLastValueReceivedOnFace(f) == SHOW_RESET) {
      // Reset game as petal. Note this will be repeated as long as RESET is in process, but it is idempotent
      weAreCenter=false;
      setValueSentOnAllFaces(SIGNAL_IDLE);
      centerFace=f;
      setValueSentOnFace( SIGNAL_READY , centerFace );
      currentLevel=0;
      scoreboard_cycle=0;
      scoreboard_count_prev=0;
    }
  }

  // Reset game with us as center if...
  // We are already center and the user tripple clicks (game restart)
  // We are not center but we found ourselves with 6 neighboors (autostart)

  if ( (buttonMultiClicked() || !weAreCenter) && doWeHave6Neighboors() ) {
    // Manual game reset where we have 6 neighboors and user longpresses
    resetGameBecomeCenter();
    return;
  }

  // OK, if we get here then we are in stable gameplay so handle normal stuff

  if (weAreCenter) {
    updateStateCenter();
  } else {
    updateStatePetal();
  }

}
