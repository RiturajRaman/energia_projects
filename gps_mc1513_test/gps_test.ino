#define SUCCESS 0
#define FAILURE -1

#define BAUD_DESD_GPS 57600

#define BAUD_RATE_CHANGE


int baud_num = 6;
int baud_arr[] = {4800, 9600, 19200, 38400, 57600, 115200};

int isValid (char character)
{
  int asc = (int)character;
  if ( (asc >= 48 && asc <= 57) || (asc >= 65 && asc <= 90) || 
      (asc >= 97 && asc <= 122) || (asc == 10) || 
      (asc == 13) || (character == ',') || 
      (character == '*') || (character == '$') || 
      (character == '.') )
    return SUCCESS;
   else 
   {
      Serial.println ("Katta");
      return FAILURE;
   }
 
}

int try_baud (int baud)
{
    char incomingByte;
    int num_try = 0;

    Serial.println ("**************************************");
    Serial.print ("Going to try with baud: ");
    Serial.println (baud);

    Serial1.begin(baud); //open port at requested baud rate
    delay (1*1000);

    while (num_try++ <= 1000)
    {
        while ( Serial1.available() > 0 )
        {
              // read the incoming byte:
              incomingByte = Serial1.read();
              //Serial.print(incomingByte); //just for fun, will comment out

              if (isValid(incomingByte) == FAILURE)  //not a valid character in this context, 
                                                                        //junk data, wrong baud. RUN!!!
              {
                  Serial.println ("Got a wrong character");
                  return FAILURE;
              }
      
          }  
    }
    //Seems we got all correct characters, this baud rate should be correct (Hope so!)
    num_try = 0;
    Serial1.end();
    Serial.println ("***********************************");
    return SUCCESS;
}

int check_gps_baud ()
{
  int i = 0;
  int baud_cur = 0;

  while ( try_baud ( baud_arr[i++] ) != SUCCESS ) //keep trying baud rates till success
  {
      if (i >= baud_num) //Baud rates finished, none matched
      {
        Serial.println ("No baud match!!");
        return FAILURE;
      }
  }
  //Out of while, baud rate has matched
  baud_cur = baud_arr[ i - 1 ];
  Serial.print ("GPS Baud is: ");
  Serial.println (baud_cur);

  #ifdef BAUD_RATE_CHANGE
      if (baud_cur != BAUD_DESD_GPS)
      {
        Serial.print ("Undesired baud, changing to: ");
        Serial.print(BAUD_DESD_GPS);
        Serial.println ("...");

        Serial1.begin(baud_cur);

        delay(1*1000);
        if (BAUD_DESD_GPS == 57600)
          Serial1.write("$PMTK251,57600*2C\r\n");
        else if (BAUD_DESD_GPS == 4800)
          Serial1.write("$PMTK251,4800*14\r\n");
        else if (BAUD_DESD_GPS == 9600)
          Serial1.write("$PMTK251,9600*17\r\n");
        else if (BAUD_DESD_GPS == 19200)
          Serial1.write("$PMTK251,19200*22\r\n");
        else if (BAUD_DESD_GPS == 38400)
          Serial1.write("$PMTK251,38400*27\r\n");
        else if (BAUD_DESD_GPS == 57600)
          Serial1.write("$PMTK251,57600*2C\r\n");
        else if (BAUD_DESD_GPS == 115200)
          Serial1.write("$PMTK251,115200*1F\r\n");
        else
          Serial.println ("Please use a correct baud rate");
        delay (5*1000);
        Serial.println ("Done");

        Serial1.end();

        check_gps_baud(); //check baud rate again, has it changed?
      }
  #endif
  return SUCCESS; //at least we have found a matching baud rate :)

  
}

void setup()
{
  int ret;
  // start serial port at 115200 bps:
  Serial.begin(115200);
  
  //Check gps_baud rate and change it to 57600 if necessary
  ret = check_gps_baud();
  Serial.println ("Done with setup");
}

void loop()
{
   
}

