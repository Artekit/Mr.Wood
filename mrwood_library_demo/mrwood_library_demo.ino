/*
 * This is a demo showing some effects on 20 chained AK-MrWood-L boards.
 * You can see what this code does here: http://youtu.be/SQQYvfe4TBw
 *
 * http://www.artekit.eu/products/accessories/ak-mr-wood-l/
 */
 
#include <MrWood.h>

byte dimm = 0;
byte up_down = 1;

void setup() {
  mrwood_init(20, 13, 11, 12, 10);
}

void loop() {
  
  int i, b, speed;
  char *included = " dedulcnI";
  
  mrwood_clear();
  mrwood_print_align("Arduino Library", 0);
  delay(2000);
  
  while (*included)
  {
     mrwood_print(*(included++));
     delay(100);
  }
  delay(2000);

  mrwood_clear();
  mrwood_print_align("Left justified", 0);
  delay(2000);
  mrwood_clear();
  mrwood_print_align("Right justified", 1);
  delay(2000);
   
  mrwood_clear();
  mrwood_print_align("Fade In. Fade Out.", 0);
 
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
  
    mrwood_dimmer(dimm);  
    delay(2);
  }
  
  mrwood_clear();
  mrwood_print_align("ASCII Table", 0);
  delay(2000); 
  
  speed = 120;
  
  for (b = 0; b < 4; b++)
  {
    for (i = 0x20; i < 0x7A; i++)
    {
      mrwood_print(i);
      delay(speed--);
      if (speed < 10) speed = 10;
    }
  }
  
  mrwood_clear();
  mrwood_print_align("  Meet Mr. Wood", 0);
  delay(7000); 
}
