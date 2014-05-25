/*
 * This is a demo showing some effects on 20 chained AK-MrWood-L boards.
 * You can see what this code does here: http://youtu.be/SQQYvfe4TBw
 *
 * http://www.artekit.eu/products/accessories/ak-mr-wood-l/
 */
#include <MrWood.h>

/*
 * Initialize MrWood.
 * 20 displays, SRCLK on pin 13, SCLK on pin 11,
 * SER on pin 12, OE on pin 10.
 */
Artekit_MrWood MrWood(20, 13, 11, 12, 10);

/* We are buffering data into this array */
byte dataBuffer[22];

/* We use this little table of number instead of the one in the library */
byte numbersTable[16] = {
	0x3F,			/* 0 */
	0x06,			/* 1 */
    0x5b,			/* 2 */
	0x4f,			/* 3 */
	0x66,			/* 4 */
	0x6d,			/* 5 */
	0x7d,			/* 6 */
	0x07,			/* 7 */
	0x7f,			/* 8 */
	0x6f,			/* 9 */
};

/* We store counters in these arrays than then we show in the
 * effect4Counters(), effect4CountersBlink() and in effectMovingCounter().
 */
byte counter1[4];
byte counter2[4];
byte counter3[4];
byte counter4[4];
byte counter5[4]; 

/* Increments a counter */
static void incrementCounter(byte *counter) {

  if (++counter[3] > 9) {
    counter[3] = 0;
    if (++counter[2] > 9) {
      counter[2] = 0;
      if (++counter[1] > 9) {
        counter[1] = 0;
        if (++counter[0] > 9) {
          counter[0] = 0;
        }
      }
    }
  }
}

/* Copies a counter into the dataBuffer at a given position */
static void copyCounterToBuffer(uint8_t pos, uint8_t* counter)
{
	int n, r = 3;
	for (n = 0; n < 4; n++) {
      dataBuffer[pos++] = numbersTable[counter[r--]];
    }
}

/* This functions clears the dataBuffer */
void ClearBuf(void) {
  int i;
  for (i = 0; i < 20; i++) dataBuffer[i] = 0;
}

/* This function increments a number on the last display
 * while filling the others with zero.
 */
void effectZeroesAndIncrement() {
  int i;        /* Count digit position 0 to 19 */
  int cnt = 0;  /* Digit value from 0 to 9 */
  
  MrWood.clear();
  ClearBuf();
  
  for (i = 0; i < 20; i++) {
    dataBuffer[i] = numbersTable[0];
    dataBuffer[0] = numbersTable[cnt];
    if (++cnt > 9) cnt = 0;
      MrWood.printBytes(dataBuffer, 20);
      delay(50);
    }  
}

/* This function shows a counter (from 0 to 9) moving to the right */
void effectSingleCounterRight(void) {
  int cnt = 0;  /* Digit value from 0 to 9 */
  int i;        /* Count digit position 0 to 19 */

  ClearBuf();
  MrWood.clear();

  for (i = 19; i >= 0; i--) {
    ClearBuf();
    dataBuffer[i] = numbersTable[cnt];
    if (++cnt > 9) cnt = 0;
    MrWood.printBytes(dataBuffer, 20);
    delay(75);
   }
}

/* This function shows a counter (from 0 to 9) moving to the left */
void effectSingleCounterLeft(void)
{
  int  cnt = 0;  /* Digit value from 0 to 9 */
  int  i;        /* Count digit position 0 to 19 */

  ClearBuf();
  MrWood.clear();

  for (i = 0; i < 20; i++) {
    ClearBuf();
    dataBuffer[i] = numbersTable[cnt];
    if (++cnt > 9) cnt = 0;
    MrWood.printBytes(dataBuffer, 20);
    delay(75);
  }
}

/* This function fills all the displays with progressive
 * numbers between 0 and 9 */
void effectProgressiveNumbers(void) {
  int cnt = 0;  /* Digit value from 0 to 9 */
  int i;        /* Count digit position 0 to 19 */

  ClearBuf();
  MrWood.clear();

  for (i = 19; i >= 0; i--) {
    dataBuffer[i] = numbersTable[cnt];
    if (++cnt > 9) cnt = 0;
    MrWood.printBytes(dataBuffer, 20);
    delay(75);
  }
}

/* This function shows 4 counters incrementing at the same time */
void effect4Counters(void) {
  int cnt1 = 0;      /* Count block 1 */
  int cnt2 = 0;      /* Count block 2 */
  int cnt3 = 0;      /* Count block 3 */
  int i, n; /* Auxiliar variables */
  
  ClearBuf();
  MrWood.clear();
  
  /* Repeat the effect 100 times */
  for (i = 0; i < 100; i++) {
  
	/* Process block 1 (always) */
    incrementCounter(counter1);

    /* If count reachs 5 increment block 2 */
    if (++cnt1 == 5) {
      cnt1 = 0;
      incrementCounter(counter2);
    }

    /* If count reachs 10 increment block 3 */    
    if (++cnt2 == 10) {
      cnt2 = 0;
      incrementCounter(counter3);
    }

    /* If count reachs 10 increment block 4 */    
    if (++cnt3 == 20) {
      cnt3 = 0;
      incrementCounter(counter4);
    }
	
	/* Copy counter 1 to the buffer */
	copyCounterToBuffer(0, counter1);
	/* Copy counter 2 to the buffer */
	copyCounterToBuffer(5, counter2);
	/* Copy counter 3 to the buffer */
	copyCounterToBuffer(10, counter3);
	/* Copy counter 4 to the buffer */
	copyCounterToBuffer(15, counter4);

    /* Display buffer */
    MrWood.printBytes(dataBuffer, 20);
    delay(50);
  }
}

/* The same as effect4Counters but with blocks 2 and 3 blinking */
void effect4CountersBlink(void) {

  int cnt1 = 0;        /* Count block 1 */
  int cnt2 = 0;        /* Count block 2 */
  int cnt3 = 0;        /* Count block 3 */
  int cnt4 = 0;        /* Count block 4 */
  int blk = 1;         /* Blink flag */
  int i, n, r, pos;    /* Auxiliar variables */

  ClearBuf();
  MrWood.clear();

  /* Repeat effect 200 times */  
  for (i = 0; i < 200; i++) {

	/* Process block 1 (always) */
    incrementCounter(counter1);

    /* If count reachs 5 increment block 2 */
    if (++cnt1 == 5) {
      cnt1 = 0;
      incrementCounter(counter2);
    }

    /* If count reachs 10 increment block 3 */    
    if (++cnt2 == 10) {
      cnt2 = 0;
      incrementCounter(counter3);
    }

    /* If count reachs 10 increment block 4 */    
    if (++cnt3 == 20) {
      cnt3 = 0;
      incrementCounter(counter4);
    }

    /* Increment blink counter and update blink flag */
    if (++cnt4 > 6) cnt4 = 0;
    if (cnt4 < 3) {
      blk = 0;
    } else {
      blk = 1;
    }

	/* Copy counter 1 to the buffer */
	copyCounterToBuffer(0, counter1);
	
	/* If not blinking, copy counters 2 and 3*/
	if (blk) {
		/* Copy counter 2 to the buffer */
		copyCounterToBuffer(5, counter2);
		/* Copy counter 3 to the buffer */
		copyCounterToBuffer(10, counter3);
	} else {
		/* Otherwise fill with counters 2 and 3 with empty spaces */
		pos = 5;
		for (n = 0; n < 4; n++) {
			dataBuffer[pos++] = 0;
		}
		pos = 10;
		for (n = 0; n < 4; n++) {
			dataBuffer[pos++] = 0;
		}
	}
	
	/* Copy counter 4 to the buffer */
	copyCounterToBuffer(15, counter4);

    /* Display buffer */
    MrWood.printBytes(dataBuffer, 20);
    delay(50);
  }
}

/* This function shows a quick counter moving around */
void effectMovingCounter(void) {
  int cnt = 0;
  int cnt1 = 0;
  int shift = 0;
  int flip = 0;
  int i, pos;

  ClearBuf();
  MrWood.clear();

  /* Repeat effect 300 times */
  for (i = 0; i < 300; i++) {

    /* Start with a clear buffer */
    ClearBuf();
    pos = shift;

    /* Copy buffer in positio 3 to 0 (right most) */
	copyCounterToBuffer(pos, counter5);

    /* Increment buffer */
    incrementCounter(counter5);

    /* Shift count 1digit position left */
    if (++cnt1 > 9) {
      cnt1 = 0;
      if (++cnt > 16) {
      if (flip) {
        flip = 0;
      } else {
        flip = 1;
      }
      cnt = 0;
    }

    /* If flip = 1 move to left, if flip = 0 move to right */
    if (flip) {
      --shift;
    } else {
      ++shift;
    }
   }

    /* Display results */
    MrWood.printBytes(dataBuffer, 20);
    delay(20);
  }
}

/* Show two progress bars. One on top and one on the bottom.
   The bottom bar increments when the top bar reachs the maximum value */
void effectProgessBars(void) {
  int cnt = 19;
  int cnt1 = 0;
  int i, n = 19, r, j;

  ClearBuf();
  MrWood.clear();
  
  /* For every display show vertical lines on the top half.
	 First lit the F segment and then the B segment.
   */

  for (r = 0; r < 20; r++) {
    n = 19;
	
	/* Fill the top bar */
    for (i = 39; i >= 0; i--) {
	
	  /* Check if we need to lit F segment */
      if (i & 1) {
	    /* Lit B segment, then move to the next display */
        dataBuffer[n] |= 0x22;
        --n;
      } else {
	    /* Lit F segment */
        dataBuffer[n] |= 0x20;
      }
	  
	  /* Print the buffer */
      MrWood.printBytes(dataBuffer, 20);
      delay(5);
    }

	/* Bottom progress bar fille when the above loop completes */
    dataBuffer[cnt--] |= 0x14;
    
	/* Fill the bttom progress bar with empty spaces if there is
	   nothing to show */
    for (j = 0; j < 19; j++)
		dataBuffer[j] &= 0x14;
    
	/* Print the buffer */
	MrWood.printBytes(dataBuffer, 20);
  }
}

/* Turn every segment on all the displays and do some blinking */
void effectBlinker(void) {
  int i;

  for (i = 0; i < 20; i++) dataBuffer[i] = 0xff;
    MrWood.printBytes(dataBuffer, 20);
    for (i = 0; i < 5; i++) {
      MrWood.enable();
      delay(500);
      MrWood.disable();
      delay(500);
    }
}

/* Turn every segment on all the displays and apply PWM dimmer */
void effectDimmer(void) {
  byte dimm = 0;
  byte up_down = 1;
  int i;

  for (i = 0; i < 3000; i++)
  {
    if (up_down)
    {
      dimm++;
      
      if (dimm == 100)
        up_down = 0;
    } else {
       dimm--; 
       if (dimm == 0)
        up_down = 1;
    }
  
    MrWood.dimmer(dimm);  
    delay(2);
  }
}

/* Setup */
void setup() {
  
}

/* Loop */
void loop() {

  effectZeroesAndIncrement();
  effectSingleCounterRight();
  effectSingleCounterLeft();
  effectProgressiveNumbers();
  effect4Counters();
  effect4CountersBlink();
  effectMovingCounter();
  effectProgessBars();
  effectBlinker();
  effectDimmer();
}

