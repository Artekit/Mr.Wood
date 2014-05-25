/*
 * A very simple demo on four chained AK-MrWood-L boards.
 * http://www.artekit.eu/products/accessories/ak-mr-wood-l/
 *
 * It counts up to 9999 and then resets the counter.
 */

#include <MrWood.h>

/*
 * Initialize MrWood.
 * 4 displays, SRCLK on pin 13, SCLK on pin 11,
 * SER on pin 12, OE on pin 10.
 */
Artekit_MrWood MrWood(4, 13, 11, 12, 10);

int count = 1;

void setup() {
  /* Nothing to initialize */
}

void loop() {
  
  /* Clear the displays */
  MrWood.clear();
  
  /* Print and increment */
  MrWood.printInt(count++);
  
  /* Check if we get to 9999 */
  if (count == 9999) count = 0;
  
  /* Delay a second */ 
  delay(1000);
}
