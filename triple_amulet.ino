// Import necessary libraries for SPI communication and RFID card reading
#include <SPI.h>
#include <RFID.h>

// Define pins for the relay and reset
const int ROLE_PIN = 8;
const int RESET_PIN = 10;

// Define the IDs of the required cards
const String REQUIRED_CARD_1 = "218112224128202";
const String REQUIRED_CARD_2 = "1056746195199";
const String REQUIRED_CARD_3 = "1056746195199";

// Initialize RFID card readers
RFID cardReader1(2, 9);
RFID cardReader2(3, 9);
RFID cardReader3(4, 9);

void setup()
{
  // Start the serial communication and SPI
  Serial.begin(9600);
  SPI.begin();

  // Initialize the card readers
  cardReader1.init();
  cardReader2.init();
  cardReader3.init();

  // Set the role and reset pins as outputs
  pinMode(ROLE_PIN, OUTPUT);
  pinMode(RESET_PIN, OUTPUT);

  // Set the role and reset pins to HIGH initially
  digitalWrite(ROLE_PIN, HIGH);
  digitalWrite(RESET_PIN, HIGH);

  // Indicate that setup has completed
  Serial.println("Setup completed");
}

void loop()
{
  // Check if any of the card readers is reading a card
  if (cardReader1.isCard() || cardReader2.isCard() || cardReader3.isCard())
  {
    // Check if any of the card readers has read a card's serial number
    if (cardReader1.readCardSerial() || cardReader2.readCardSerial() || cardReader3.readCardSerial())
    {
      // Obtain the IDs of the cards read by the readers
      String cardID1 = getCardID(cardReader1);
      String cardID2 = getCardID(cardReader2);
      String cardID3 = getCardID(cardReader3);

      // Print out the IDs of the cards read
      Serial.println("Card IDs: Card1: " + cardID1 + " Card2: " + cardID2 + " Card3: " + cardID3);

      // Check if the cards read match the required cards
      if ((cardID1 == REQUIRED_CARD_1) && (cardID2 == REQUIRED_CARD_2) && (cardID3 == REQUIRED_CARD_3))
      {
        // If the cards match, unlock for a certain duration and print a message
        unlockForDuration(5000);
        Serial.println("Access granted");
      }
    }
  }

  // Halt the card readers
  cardReader1.halt();
  cardReader2.halt();
  cardReader3.halt();
}

String getCardID(RFID &cardReader)
{
  // Initialize the card ID as an empty string
  String cardID = "";

  // Loop over the serial number array of the card reader to construct the card ID
  for (byte i = 0; i < 5; i++)
  {
    cardID += String(cardReader.serNum[i]);
  }

  // Return the constructed card ID
  return cardID;
}

void unlockForDuration(int duration)
{
  // Set the role pin to LOW to unlock
  digitalWrite(ROLE_PIN, LOW);

  // Wait for the specified duration
  delay(duration);

  // Reset the system by setting the reset pin to LOW
  digitalWrite(RESET_PIN, LOW);
}
