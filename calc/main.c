#include <curses.h>

void draw_calculator(double result, double current_num, char op) {
  clear();
  mvprintw(1, 2, "=========================");
  mvprintw(2, 2, "|    NCURSES CALC      |");
  mvprintw(3, 2, "=========================");

  mvprintw(5, 2, " [ Active Op: %c ]", op ? op : ' ');
  mvprintw(6, 2, " [ Current ]: %.4g", current_num);
  mvprintw(7, 2, " [ Result  ]: %.4g", result);
  mvprintw(8, 2, "=========================");

  mvprintw(10, 2, "Controls:");
  mvprintw(11, 2, "  0-9 : Enter digits");
  mvprintw(12, 2, "  + - * / : Operations");
  mvprintw(13, 2, "  Enter   : Calculate (=)");
  mvprintw(14, 2, "  C       : Clear display");
  mvprintw(15, 2, "  Q       : Quit application");
  mvprintw(17, 2, "Enter Input: ");

  refresh();
}

int main(void) {
  initscr();
  cbreak();
  noecho();

  keypad(stdscr, TRUE);

  double result = 0.0;
  double current_num = 0.0;
  char current_op = 0.0;
  int input;

  while (1) {
    draw_calculator(result, current_num, current_op);
    input = getch();

    if (input == 'q' || input == 'Q') {
      break;
    }

    if (input == 'c' || input == 'C') {
      result = 0.0;
      current_num = 0.0;
      current_op = 0;
      continue;
    }

    if (input >= '0' && input <= '9') {
      current_num = (current_num * 10) + (input - '0');
    } else if (input == '+' || input == '-' || input == '*' || input == '/') {
      if (current_op == 0) {
        result = current_num;
      } else {
        if (current_op == '+')
          result += current_num;
        if (current_op == '-')
          result -= current_num;
        if (current_op == '*')
          result *= current_num;
        if (current_op == '/') {
          if (current_num != 0)
            result /= current_num;
        }
      }
      current_op = input;
      current_num = 0.0;
    } else if (input == '\n' || input == KEY_ENTER) {
      if (current_op == '+')
        result += current_num;
      else if (current_op == '-')
        result -= current_num;
      else if (current_op == '*')
        result *= current_num;
      else if (current_op == '/') {
        if (current_num != 0)
          result /= current_num;
      }
      current_num = result;
      current_op = 0;
      mvprintw(19, 2, "Result: %f", result);
    }
  }

  endwin();
  return 0;
}
