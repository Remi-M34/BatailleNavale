#include "../include/window.h"
#include "../include/config.h"
#include "../include/grille.h"
#include <iostream>

#define H getHeightGrille()
#define W getWidthGrille()

using namespace std;

void init_colors(void)
{
  start_color();
  init_pair(WBLACK, COLOR_WHITE, COLOR_BLACK);
  init_pair(WCYAN, COLOR_WHITE, COLOR_CYAN);
  init_pair(WBLUE, COLOR_WHITE, COLOR_BLUE);
  init_pair(WYELLOW, COLOR_WHITE, COLOR_YELLOW);
  init_pair(WGREEN, COLOR_WHITE, COLOR_GREEN);
  init_pair(WMAGENTA, COLOR_WHITE, COLOR_MAGENTA);
  init_pair(WRED, COLOR_WHITE, COLOR_RED);
  init_pair(BWHITE, COLOR_BLACK, COLOR_WHITE);
  init_pair(BCYAN, COLOR_BLACK, COLOR_CYAN);
  init_pair(BBLUE, COLOR_BLACK, COLOR_BLUE);
  init_pair(BYELLOW, COLOR_BLACK, COLOR_YELLOW);
  init_pair(BGREEN, COLOR_BLACK, COLOR_GREEN);
  init_pair(BMAGENTA, COLOR_BLACK, COLOR_MAGENTA);
  init_pair(BRED, COLOR_BLACK, COLOR_RED);
  
  init_pair(CBLACK, COLOR_CYAN, COLOR_BLACK);
  init_pair(BLUEBLACK, COLOR_BLUE, COLOR_BLACK);
  init_pair(YBLACK, COLOR_YELLOW, COLOR_BLACK);
  init_pair(GBLACK, COLOR_GREEN, COLOR_BLACK);
  init_pair(MBLACK, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(RBLACK, COLOR_RED, COLOR_BLACK);
  init_pair(WHITEBLACK, COLOR_WHITE, COLOR_BLACK);
  init_pair(RBLUE, COLOR_RED, COLOR_BLUE);
}

void startProgramX()
{
  initscr();                         // initialize curses
  cbreak();                          // pass key presses to program, but not signals
  noecho();                          // don't echo key presses to screen
  keypad(stdscr, TRUE);              // allow arrow keys
  mousemask(ALL_MOUSE_EVENTS, NULL); /* Report all mouse events */

  timeout(0);  // no blocking on getch()
  curs_set(0); // set the cursor to invisible

  init_colors();
}

void stopProgramX()
{
  refresh();
  getch();
  endwin();
}

void Window::update() const
{
  wrefresh(win);
  wrefresh(frame);
  refresh();
}

Window::Window(int h, int w, int x, int y, char c)
    : height(h), width(w), startx(x + COLS / 2), starty(y + LINES / 2), bord(c)
{
  colorwin = WBLACK;
  colorframe = WBLACK;
  frame = newwin(h + 2, w + 2, starty, startx);
  win = subwin(frame, h, w, starty + 1, startx + 1);
  wbkgd(frame, COLOR_PAIR(colorwin));
  wbkgd(win, COLOR_PAIR(colorframe));
  wborder(frame, c, c, c, c, c, c, c, c);
  wattron(win, COLOR_PAIR(colorwin));
  wattron(frame, COLOR_PAIR(colorframe));
  update();
  if (h == H && w == W * 2)
  {
    estGrille = true;
  }
  else
  {
    estGrille = false;
  }
}

Window::~Window()
{
  wborder(frame, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
  wattroff(win, COLOR_PAIR(colorwin));
  wattroff(win, COLOR_PAIR(colorframe));
  werase(win);
  update();
  delwin(win);
}

void Window::print(int x, int y, std::string s, Color c) const
{

  wattron(win, COLOR_PAIR(c));
  mvwprintw(win, y, x, s.c_str());
  wattroff(win, COLOR_PAIR(c));
  update();
}
void Window::print(int x, int y, char s, Color c) const
{
  wattron(win, COLOR_PAIR(c));
  mvwaddch(win, y, x, s);
  wattroff(win, COLOR_PAIR(c));
  update();
}
void Window::print(int x, int y, std::string s) const
{
  mvwprintw(win, y, x, s.c_str());
  update();
}

void Window::printborderch(int x, int y, char s)
{

  mvwaddch(frame, y, x, s);
  mvwaddch(frame, y, x + 1, ' ');
}

void Window::printborder(int x, int y, string s)
{
  mvwprintw(frame, y, x, s.c_str());
}

void Window::BordureNumerotee(int x, int y)
{
  wattron(frame, A_DIM);
  wattron(frame, A_INVIS);

  const char *Alphabet = "0ABCDEFGHIJKLMNOPQRST";
  for (int i = 1; i <= W; i++)
  {
    if (x == i - 1)
    {
      wattron(frame, A_UNDERLINE);

      wattron(frame, COLOR_PAIR(WRED));
      mvwaddch(frame, 0, 2 * i - 1, ACS_BULLET);
      mvwaddch(frame, 0, 2 * i, Alphabet[i]);
      mvwaddch(frame, 0, 2 * i + 1, ACS_BULLET);
      wattron(frame, COLOR_PAIR(colorframe));
      wattroff(frame, A_UNDERLINE);
    }
    else

      printborderch(2 * i, 0, Alphabet[i]);
  }
  if (x != 0)
    mvwaddch(frame, 0, 1, ' ');

  for (int i = 1; i <= H; i++)
  {
    if (i - 1 == y)
    {
      wattron(frame, A_UNDERLINE);

      wattron(frame, COLOR_PAIR(WRED));
      printborder(0, i, myitoa((i > 9 ? (i == 10 || i == 20 ? i / 10 : i % 10) : i)));
      wattroff(frame, A_UNDERLINE);

      wattron(frame, COLOR_PAIR(colorframe));
    }
    else
      printborder(0, i, myitoa((i > 9 ? (i == 10 || i == 20 ? i / 10 : i % 10) : i)));
  }
  update();
}

void Window::print(int x, int y, char s) const
{
  mvwaddch(win, y, x, s);
  update();
}

int Window::getX() const { return startx; }
int Window::getY() const { return starty; }
int Window::getHauteur() const { return height; }
int Window::getLargeur() const { return width; }
Color Window::getCouleurBordure() const { return colorframe; }
Color Window::getCouleurFenetre() const { return colorwin; }

void Window::setCouleurBordure(Color c)
{
  colorframe = c;
  wattron(frame, COLOR_PAIR(colorframe));
  wborder(frame, bord, bord, bord, bord, bord, bord, bord, bord);
  if (estGrille)
  {
    BordureNumerotee(-1, -1);
  }
  else
    update();
}

void Window::setBordureFine()
{
  wborder(frame, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
  colorwin = CBLACK;
  wbkgd(frame, COLOR_PAIR(colorwin));
  wattron(win, COLOR_PAIR(colorwin));
  // BordureNumerotee();
  update();
}

void Window::setCouleurFenetre(Color c)
{
  colorwin = c;
  wattron(win, COLOR_PAIR(colorwin));
  wbkgd(win, COLOR_PAIR(colorwin));
  update();
}

void Window::setCarBordure(char c)
{
  wborder(frame, c, c, c, c, c, c, c, c);
  bord = c;
  if (estGrille)
  {
    BordureNumerotee(-1, -1);
  }
}

int Window::getWindowWidth()
{
  return width;
}

void Window::clear() const
{
  werase(win);
  update();
}

void Window::clearall() const
{
  werase(frame);
  update();
}

WINDOW *Window::getWin()
{
  return win;
}