

#include <TouchScreen.h>
#include <MCUFRIEND_kbv.h>

#define MINPRESSURE 200
#define MAXPRESSURE 1000

char *name = "0x4747";  //edit name of shield
const int XP = 6, XM = A2, YP = A1, YM = 7; //ID=0x9341
// const int TS_LEFT = 170, TS_RT = 949, TS_TOP = 170, TS_BOT = 921;
const int TS_LEFT = 189, TS_RT = 944, TS_TOP = 189, TS_BOT = 921;

MCUFRIEND_kbv tft;       // hard-wired for UNO shields anyway.
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
TSPoint tp;


int16_t BOXSIZE;
int16_t PENRADIUS = 0;
uint16_t ID, oldcolor, currentcolor;
uint8_t Orientation = 1;    //PORTRAIT
int i = 0 ;
int flag = 1 ;

#define ix 1 ;
#define efex -1 ;
int player = -1 ;


// Assign human-readable names to some common 16-bit color values:
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
int colors[8] = { BLACK, BLUE, RED, GREEN, CYAN, MAGENTA, YELLOW, WHITE } ;
byte board_array[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 } ;
int start[] = {120, 145, 100, 50 } ;

void setup(void)
{
  Serial.begin(9600) ;
  uint16_t tmp;

  tft.reset();
  ID = tft.readID();
  tft.begin(ID);
  tft.setRotation(Orientation);
  tft.fillScreen(BLACK);
  //  delay(1000);
  Serial.println(" __ init __" ) ;
  tft.setTextColor(GREEN);
  tft.setTextSize(2);
  tft.setCursor(96, 80) ;
  tft.print("Tic Tac Toe" ) ;
  tft.drawRect(start[0], start[1], start[2], start[3], WHITE) ;
  //  Serial.println("drawing start box") ;
  tft.setCursor(140, 165) ;
  tft.setTextColor(YELLOW) ;
  tft.print("Start") ;

  while (flag)
  {
    tp = ts.getPoint();
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    if (tp.z < MINPRESSURE || tp.z > MAXPRESSURE) continue ;
    if ( tp.x > 650 && tp.x < 780 && tp.y > 460 && tp.y < 666 ) break ;

  }

  flag = 0 ;
  tft.fillScreen(BLACK);
  board() ;
}


void board(void)
{

  int width_of_row = 6 ;
  int width_of_col = 6 ;
  int i = 0 ;
  int rows1[] = { 60, 80, 270, 80 } ;
  int rows2[] = { 60, 160, 270, 160 }   ;
  int cols1[] = { 120, 30, 120, 210 } ;
  int cols2[] = { 205, 30, 205, 210  }   ;

  for ( i = 0 ; i < width_of_row ; i++ )
  {
    tft.drawLine(rows1[0], rows1[1] + i, rows1[2], rows1[3] + i , WHITE ) ;
  }
  for ( i = 0 ; i < width_of_row ; i++ )
  {
    tft.drawLine(rows2[0], rows2[1] + i, rows2[2], rows2[3] + i , WHITE ) ;
  }
  for ( i = 0 ; i < width_of_col ; i++ )
  {
    tft.drawLine(cols1[0] + i, cols1[1] , cols1[2] + i, cols1[3] , WHITE ) ;
  }
  for ( i = 0 ; i < width_of_col ; i++ )
  {
    tft.drawLine(cols2[0] + i, cols2[1] , cols2[2] + i, cols2[3] , WHITE ) ;
  }
  //   tft.fillCircle(100, 100, 10, RED) ;
}


int game(int player)
{
  tft.fillRect(1, 1, 300, 20, BLACK ) ;
  tft.setTextSize(2) ;
  tft.setTextColor(YELLOW) ;
  if ( player > 0 )
  {
    tft.setCursor(5, 5 ) ;
    tft.print("  ") ;
    tft.print("X") ;
  }
  if ( player < 0 )
  {
    tft.setCursor(5, 5 ) ;
    tft.print("  ") ;
    tft.print("O") ;
  }
  tft.setTextSize(1) ;
  tft.setTextColor(BLUE) ;
  tft.print("  Touch your selected position") ;
  Serial.println("awaiting player..") ;
  while (1) {
    tp = ts.getPoint();
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);

    if (tp.z < MINPRESSURE || tp.z > MAXPRESSURE) continue;
    player = analize_game(tp.x, tp.y, player) ;

    return (player) ;
  }
}

void draw_player( int x, int y, int player)
{
  uint16_t xpos, ypos;

  xpos = map( tp.x, 170, 983, 0, 240 ) ;
  ypos = map(tp.y, 170, 920 , 0, 320) ;

  if ( player < 0 )
  {
    Serial.println("drawing O") ;
    tft.drawCircle(ypos, xpos, 20, RED) ;
  }
  else
  {
    Serial.println("drawing X") ;
    tft.drawLine(ypos, xpos, ypos + 20, xpos + 20, BLUE) ;
    tft.drawLine(ypos + 20, xpos, ypos, xpos + 20, BLUE) ;
  }
}


int analize_game(int x, int y, int player)
{
  uint16_t xpos, ypos;
  Serial.println("analizing .. ") ;
  int boardy[2] = {470, 660 } ; // rows
  int boardx[2] = {470, 715 } ; // cols

  if ( y > 280 && y <  780 && x > 266 && x < 860 ) // check if inside game board area
  {
    if ( y < boardx[0] && x < boardy[0] )
    {
      if ( board_array[0] == 0 )
      {
        board_array[0] = player ;
        draw_player(  x,  y,  player) ;
        player = player * (-1) ;
      }
    }
    else if ( y > boardx[0] && y < boardx[1] && x < boardy[0]  )
    {
      if ( board_array[1] == 0 )
      {
        board_array[1] = player ;
        draw_player(  x,  y,  player) ;
        player = player * (-1) ;
      }

    }
    else if ( y > boardy[1] &&  x < boardy[0])
    {
      if ( board_array[2] == 0 )
      {
        board_array[2] = player ;
        draw_player(  x,  y,  player) ;
        player = player * (-1) ;
      }
    }
    else if (  y < boardx[0] && x < boardy[1] && x > boardy[0] )
    {
      if ( board_array[3] == 0 )
      {
        board_array[3] = player ;
        draw_player(  x,  y,  player) ;
        player = player * (-1) ;
      }
    }
    else if (  y > boardx[0] && y < boardx[1] && x < boardy[1] && x > boardy[0] )
    {
      if ( board_array[4] == 0 )
      {
        board_array[4] = player ;
        draw_player(  x,  y,  player) ;
        player = player * (-1) ;
      }
    }
    else if (  y > boardx[1] && x < boardy[1] && x > boardy[0] )
    {
      if ( board_array[5] == 0 )
      {
        board_array[5] = player ;
        draw_player(  x,  y,  player) ;
        player = player * (-1) ;
      }
    }
    else if ( x > boardx[1] && y < boardy[0] )
    {
      if ( board_array[6] == 0 )
      {
        board_array[6] = player ;
        draw_player(  x,  y,  player) ;
        player = player * (-1) ;
      }
    }
    else if ( x > boardx[1] && y < boardy[1] && y > boardy[0] )
    {
      if ( board_array[7] == 0 )
      {
        board_array[7] = player ;
        draw_player(  x,  y,  player) ;
        player = player * (-1) ;
      }

    }
    else if ( y > boardx[1] && x > boardy[1] )
    {
      if ( board_array[8] == 0 )
      {
        board_array[8] = player ;
        draw_player(  x,  y,  player) ;
        player = player * (-1) ;
      }
    }
  }
  xpos = map(tp.x, TS_LEFT, TS_RT, 0, tft.width());
  ypos = map(tp.y, TS_TOP, TS_BOT, 0, tft.height());

  //  ypos = map( tp.y, 170, 983, 0, 240 ) ;
  // xpos = map(tp.x, 170, 920 , 0, 320) ;

  if ( ypos != boardx[0] && ypos != boardx[1] && xpos != boardy[0] && xpos != boardy[1] )
  {
    tft.setCursor(ypos - 10, xpos - 10) ;
    tft.setTextSize(3) ;
    if (player > 0 )
    {
      tft.setTextColor(GREEN) ;
    }
    else
    {
      tft.setTextColor(RED) ;
    }

    //    tft.print(player) ;
  }

  Serial.println("board array ") ;
  //  while ( i < 9 )
  //  {
  i = 0 ;
  for ( int l = 0 ; l < 3 ; l++)
  {
    for ( int r = 0 ; r < 3 ; r++ )
    {
      Serial.print(board_array[i]) ;
      Serial.print("   ") ;
      i++ ;
    }
    Serial.println("") ;
    //   }
    //  i++ ;
  }
  Serial.println("") ;
  // end debug

  // check winning
  Serial.println("checking for a winner") ;

  i = 0 ;
  for ( int l = 0 ; l < 3 ; l++)
  {
    for ( int r = 0 ; r < 3 ; r++ )
    {
      i++ ;
    }

  }


  return (player) ;
}

void loop()
{

  player = game(player) ;
  delay(1000) ;
}
