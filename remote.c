/*******************************************************************************
* Copyright (C) 2015, Jean-Yves VET, contact [at] jean-yves [dot] vet          *
*                                                                              *
* This software is licensed as described in the file LICENCE, which you should *
* have received as part of this distribution. You may opt to use, copy,        *
* modify, merge, publish, distribute and/or sell copies of the Software, and   *
* permit persons to whom the Software is furnished to do so, under the terms   *
* of the LICENCE file.                                                         *
*                                                                              *
* This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY    *
* KIND, either express or implied.                                             *
********************************************************************************
*                                                                              *
*            Frame to mimic the CAME TOP432NA remote control:                  *
*                                                                              *
*  Start bit (header)  __       10 bits code       __ Ending 2 bits (trailer)  *
*                        |  _________|_________   |                            *
*                        0 [X X X X X X X X X X] 0 1                           *
*                                                                              *
*******************************************************************************/

/* Get bit of the number n at position p */
#define  BIT(n, p)   !!(n & (1 << p - 1))

/* Arduino settings  */
#define  OUTPUT_PIN            12   /* Output pin to the transmitter.        */
#define  RESTART_DELAY_MS      5000 /* 5s delay before restart.              */
#define  SERIAL_SPEED_BAUD     9600 /* Speed for serial communication.       */

/* Frame characteristics */
#define  FRAME_NB_CODE_BITS    10   /* Number of bits in the code.           */
#define  FRAME_NB_CODE_COMB    (1 << FRAME_NB_CODE_BITS)  /* Combinations.   */
#define  FRAME_NB_REEMIT       3    /* Times the same signal is reemitted.   */
#define  FRAME_SHORT_DELAY_US  300  /* 300µs delay for short pulse.          */
#define  FRAME_LONG_DELAY_US   600  /* 600µs delay for long pulse.           */
#define  BTW_REEMIT_DELAY_MS   12   /* 12ms delay before sending same signal.*/
#define  BTW_FRAMES_DELAY_MS   50   /* 50ms delay between each combinaison.  */

#define  CODE                  375  /* CHANGE CODE HERE. Between 0 and 1023. */
//#define  SCAN_MODE                /* Uncomment to enter scan mode.         */


/**                      ___
 * Emit a 0 pulse.   ___|   |___
 */
void send_zero()
{
    digitalWrite(OUTPUT_PIN, LOW);
    delayMicroseconds(FRAME_SHORT_DELAY_US);
    digitalWrite(OUTPUT_PIN, HIGH);
    delayMicroseconds(FRAME_SHORT_DELAY_US);
    digitalWrite(OUTPUT_PIN, LOW);
    delayMicroseconds(FRAME_SHORT_DELAY_US);
}

/**                  ______
 * Emit a 1 pulse.  |      |___
 */
void send_one()
{
    digitalWrite(OUTPUT_PIN, HIGH);
    delayMicroseconds(FRAME_LONG_DELAY_US);
    digitalWrite(OUTPUT_PIN, LOW);
    delayMicroseconds(FRAME_SHORT_DELAY_US);
}

/**
 * Emit a frame (header + code + trailer).
 *
 * @param   code       Code to send (between 0 and 1023).
 * @param   nb_emit    Number of times the frame should be emitted.
 */
void send_frame(const uint32_t code, const int nb_emit)
{
    for (int i = 0; i < nb_emit; i++)
    {
        /* Send header */
        send_zero();

        /* Send code */
        for (int bit = FRAME_NB_CODE_BITS; bit > 0; bit--)
        {
            if (BIT(code, bit))
                send_one();
            else
                send_zero();
        }

        /* Send trailer */
        send_zero();
        send_one();

        delay(BTW_REEMIT_DELAY_MS);
    }
}

#ifdef SCAN_MODE
/**
 * Emit all possible frames to scan all combinations.
 */
void scan_mode()
{
    for (int i = 0; i < FRAME_NB_CODE_COMB; i++)
    {
        Serial.print("Sending code: ");
        Serial.println(i);

        delay(BTW_FRAMES_DELAY_MS);
        send_frame(i, FRAME_NB_REEMIT);
        delay(BTW_FRAMES_DELAY_MS);
    }
}
#endif

void setup()
{
    pinMode(OUTPUT_PIN, OUTPUT);     /* Set PIN in output mode. */

#ifdef SCAN_MODE
    Serial.begin(SERIAL_SPEED_BAUD);
#endif
}

void loop()
{
#ifdef SCAN_MODE
    scan_mode();
#else
    send_frame(CODE, FRAME_NB_REEMIT);
#endif
    delay(RESTART_DELAY_MS);
}

