#include <EEPROM.h>
#include <AESLib.h>

#define TOUCH_SENSOR 2
#define PASSWORD_SLOT 0
#define PASSWORD_LENGTH 16
#define ENCRYPTION_KEY "mysecurekey12345" // 16-byte key

char plaintext[PASSWORD_LENGTH] = "mypassword12345"; // Example password
char encrypted[PASSWORD_LENGTH];
char decrypted[PASSWORD_LENGTH];
bool accessGranted = false;

void setup() {
  pinMode(TOUCH_SENSOR, INPUT);
  Serial.begin(9600);
  Serial.println("Password Vault Initialized");
  
  // Encrypt and store password in EEPROM (one-time setup)
  encryptPassword(plaintext, encrypted);
  storePasswordInEEPROM(encrypted);
}

void loop() {
  if (digitalRead(TOUCH_SENSOR) == HIGH) {
    accessGranted = !accessGranted; // Simulate authentication toggle
    delay(300); // Debounce
  }
  
  if (accessGranted) {
    retrievePasswordFromEEPROM(encrypted);
    decryptPassword(encrypted, decrypted);
    Serial.print("Decrypted Password: ");
    Serial.println(decrypted);
    delay(2000); // Simulate secure delay
  } else {
    Serial.println("Access Denied. Touch sensor to unlock.");
    delay(1000);
  }
}

void encryptPassword(char* input, char* output) {
  aes128_enc_single((byte*)ENCRYPTION_KEY, (byte*)input, (byte*)output);
}

void decryptPassword(char* input, char* output) {
  aes128_dec_single((byte*)ENCRYPTION_KEY, (byte*)input, (byte*)output);
}

void storePasswordInEEPROM(char* password) {
  for (int i = 0; i < PASSWORD_LENGTH; i++) {
    EEPROM.write(PASSWORD_SLOT + i, password[i]);
  }
}

void retrievePasswordFromEEPROM(char* password) {
  for (int i = 0; i < PASSWORD_LENGTH; i++) {
    password[i] = EEPROM.read(PASSWORD_SLOT + i);
  }
}