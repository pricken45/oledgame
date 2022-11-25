/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com
*********/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

float x;
float y;

class Enemy
{
public:
  float x;
  int y;
  float direction;
  byte speed;
  bool alive;
  Adafruit_SSD1306 disp;
  Enemy(Adafruit_SSD1306 &display, int spd, bool alv)
  {
    this->disp = display;
    this->speed = spd;
    this->alive = alv;
    x = 0;
    y = 0;
    direction = 0;
    if (random(2) > 1)
    {
      this->direction = 1;
      this->x = -5;
    }
    else
    {
      this->direction = -1;
      this->x = 128 + 5;
    }

    this->y = random(64);
  };

  void render()
  {
    if (!this->alive)
      return;
    disp.drawLine(this->x, this->y, this->x + this->direction * -1 * 5, this->y, WHITE);
  };
};

const float gravity = 0.15;
float yVelocity = 0;
float xVelocity = 0;

const float jumpForce = 2.5;
const float drag = 0.96;
bool menu = true;

int lastButtonState = 0;
bool button_debounce = false;
bool button_debounceR = false;

bool startGame = false;

int score = 0;
int scoreAdder = 0;

// Enemy enemies[15] = {Enemy(display, 3, false), Enemy(display, 3, false), Enemy(display, 3, false), Enemy(display, 3, false), Enemy(display, 3, false), Enemy(display, 3, false), Enemy(display, 3, false), Enemy(display, 3, false), Enemy(display, 3, false), Enemy(display, 3, false), Enemy(display, 3, false), Enemy(display, 3, false), Enemy(display, 3, false), Enemy(display, 3, false), Enemy(display, 3, false)};

Enemy enemy(display, 1, false);

void resetPosition()
{
  x = SCREEN_WIDTH / 2;
  y = 5;
  yVelocity = 0;
  xVelocity = 0;
  score = 0;
}

void startMenu()
{
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.clearDisplay();

  while (!startGame)
  {
    display.drawRect(15, 33, 41, 15, WHITE);
    display.setCursor(3 + 33, 23);

    display.println("Press any");
    display.setCursor(3 + 15, 35 + 2);
    display.println("button to begin");
    display.display();
    if ((!digitalRead(2)) == LOW && button_debounce == false)
    {
      startGame = true;
      button_debounce = true; // toggle button debounce flag
    }

    if ((!digitalRead(2)) == HIGH && button_debounce == true)
    { // this will prevent subsequent presses
      button_debounce = false;
    }

    if ((!digitalRead(3)) == LOW && button_debounceR == false)
    {
      startGame = true;
      button_debounceR = true; // toggle button debounce flag
    }

    if ((!digitalRead(3)) == HIGH && button_debounceR == true)
    { // this will prevent subsequent presses
      button_debounceR = false;
    }
  }

  display.clearDisplay();

  display.setCursor(64, 25);
  display.println("3");
  display.display();
  tone(A2, 800, 300);

  delay(1000);
  display.clearDisplay();
  display.setCursor(64, 25);
  display.println("2");
  display.display();
  tone(A2, 800, 300);
  delay(1000);
  display.clearDisplay();
  display.setCursor(64, 25);
  display.println("1");
  display.display();
  tone(A2, 1500, 500);

  delay(1000);
  menu = false;
  resetPosition();
  scoreAdder = 0;
}

void death()
{
  startGame = false;
  tone(A2, 2000, 500);
  delay(500);
  tone(A2, 400, 500);
}

void collisionDetection()
{
  if (y >= SCREEN_HEIGHT || y <= -2 || x <= 0 || x >= SCREEN_WIDTH)
  {
    // resetPosition();
    // death();
    // startMenu();
  }
}

void clickLeft()
{
  yVelocity = -jumpForce;
  xVelocity = -2;
  tone(A2, 2000, 70);
}
void clickRight()
{
  yVelocity = -jumpForce;
  xVelocity = 2;
  tone(A2, 2000, 70);
}

void setup()
{
  pinMode(2, INPUT);
  pinMode(3, INPUT);

  // Serial.begin(115200);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  enemy.alive = true;

  startMenu();
}

void loop()
{
  if (menu)
  {
    return;
  }
  yVelocity += gravity;
  xVelocity *= drag;

  y += yVelocity;
  x += xVelocity;
  scoreAdder++;

  if (scoreAdder > 45)
  {
    scoreAdder = 0;
    score++;
  }

  // Serial.println(enemy.x);

  collisionDetection();

  if ((!digitalRead(2)) == LOW && button_debounce == false)
  {
    startGame = true;
    clickLeft();
    button_debounce = true; // toggle button debounce flag
  }

  if ((!digitalRead(2)) == HIGH && button_debounce == true)
  { // this will prevent subsequent presses
    button_debounce = false;
  }

  if ((!digitalRead(3)) == LOW && button_debounceR == false)
  {
    startGame = true;
    clickRight();
    button_debounceR = true; // toggle button debounce flag
  }

  if ((!digitalRead(3)) == HIGH && button_debounceR == true)
  { // this will prevent subsequent presses
    button_debounceR = false;
  }

  // Rendering
  display.clearDisplay();
  enemy.x += enemy.direction;
  enemy.render();
  display.setTextColor(WHITE);
  display.setCursor(61, 3);
  display.setTextSize(1.5);
  display.println(score);

  display.drawPixel(x, y, WHITE);
  delay(30);
  display.display();

  lastButtonState = digitalRead(2);

  // Serial.println(digitalRead(2));
}
