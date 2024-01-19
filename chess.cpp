#include <iostream>
#include <fstream>

using namespace std;


string board[] = {
                  "RNBQKBNR",
                  "PPPPPPPP",
                  "        ",
                  "        ",
                  "        ",
                  "        ",
                  "pppppppp",
                  "rnbqkbnr"
                };

string letters = "abcdefgh";
string error = "";


int turn = 1;

int x1;
int y1;
int x2;
int y2;

void print_board() {

    cout << "\n\n";
    cout << "\t\t\t\t";
    for (int i = 1; i <= 8; i++) {
        cout << "    " << i << "   ";
    }
    cout << endl;
    for (int i = 0; i < 8; i++) {
        cout << "\t\t\t\t";

        cout << "-----------------------------------------------------------------";


        cout << endl  << "\t\t\t\t";

        for (int j = 0; j <= 8; j++) {
            cout << "|       ";
        }

        cout << endl << "\t\t\t     ";
        cout << letters[i] << "  ";
        for (int j = 0; j <= 8; j++) {
            cout << "|   " << board[i][j] << "   ";
        }

        cout << endl  << "\t\t\t\t";

        for (int j = 0; j <= 8; j++) {
            cout << "|       ";
        }
        cout << endl;
    }
    cout << "\t\t\t\t-----------------------------------------------------------------" << endl;
    cout << "Select Option:    (P)lay    (Q)uit    re(D)raw    (H)andshake    (S)ave    (L)oad" << endl;



}

void get_input() {
    string move;
    bool valid = false;

    while (!valid) {
        valid = true;
        cout << "Enter your move (e.g: b2 c2): ";
        getline(cin, move);

        if (move[0] < 'a' || move[0] > 'h')
            valid = false;

        else if (move[1] < '1' || move[1] > '8')
            valid = false;

        else if (move[2] != ' ')
            valid = false;

        else if (move[3] < 'a' || move[3] > 'h')
            valid = false;

        else if (move[4] < '1' || move[4] > '8')
            valid = false;

    }

    x1 = move[1] - '0' - 1;
    y1 = move[0] - 'a';
    x2 = move[4] - '0' - 1;
    y2 = move[3] - 'a';

}

string p1_pieces = "rbnkqp";
string p2_pieces = "RBNKQP";

bool is_valid_piece() {

    char dst = board[y2][x2];
    char src = board[y1][x1];

    string pieces = p2_pieces;
    if (turn == 1)
        pieces = p1_pieces;

    for (size_t i = 0; i < pieces.length(); i++) {
        if (dst == pieces[i]) {
            error = "Can't kill own piece";
            return false;
        }
    }

    if (turn == 1)
        pieces = p2_pieces;
    else
        pieces = p1_pieces;

    for (size_t i = 0; i < pieces.length(); i++) {
        if (src == pieces[i]) {
            error = "Not your piece";
            return false;
        }
    }

    return true;
}


bool validate_pawn() {
    int y_src = y1;
    int y_dst = y2;
    if (turn == 1) {
        y_src = 7 - y_src;
        y_dst = 7 - y_dst;
    }

    if (y_src == 1) {
        if (x2 == x1 && y_dst-y_src == 2)
            return true;
    }

    if (x2 == x1 && y_dst-y_src == 1)
        return true;

    if (abs(x2-x1) == 1 && y_dst-y_src == 1) {
        if (board[y2][x2] != ' ')
            return true;
    }

    return false;
}

bool validate_rook() {
    int y = min(y1, y2) + 1;
    int x =  min(x1, x2) + 1;

    if (x2 == x1) {
        for (int i = y; i < max(y1, y2); i++) {
                if (board[i][x] != ' ')
                    return false;
        }
    }

    else if (y2 == y1) {
        for (int i = x; i < max(x1, x2); i++) {
            if (board[y][i] != ' ')
                return false;
        }
    }

    else
        return false;

    return true;
}

bool validate_bishop() {


    if (abs(x2-x1) != abs(y2-y1))
        return false;

    if (x2 > x1 && y2 > y1) {
        int x = x1 + 1;
        for (int i = y1 + 1; i < y2; i++) {
                if (board[i][x++] != ' ')
                    return false;
        }
    }

    else if (x2 < x1 && y2 > y1) {
        int x = x1 - 1;
        for (int i = y1 + 1; i < y2; i++) {
            if (board[i][x--] != ' ')
                return false;
        }
    }

    else if (x2 > x1 && y2 < y1) {
        int x = x1 + 1;
        for (int i = y2 + 1; i < y1; i++) {
            if (board[i][x++] != ' ')
                return false;
        }
    }

    else if (x2 < x1 && y2 < y1) {
        int x = x1 - 1;
        for (int i = y2 + 1; i < y1; i++) {
            if (board[i][x--] != ' ')
                return false;
        }
    }

    return true;
}

bool validate_knight() {
    if (abs(x2-x1) == 2 && abs(y2-y1) == 1)
        return true;

    else if (abs(x2-x1) == 1 && abs(y2-y1) == 2)
        return true;

    return false;
}

bool validate_queen() {
    return validate_bishop() || validate_rook();
}

bool validate_king() {
    if (abs(x2-x1) <= 1 && abs(y2-y1) <= 1)
        return true;

    return false;
}


bool check_move() {
    bool is_move_valid;

    if (!is_valid_piece()) {
        return false;
    }

    char piece = tolower(board[y1][x1]);

    switch (piece) {

        case 'p':
            error = "Invalid pawn move";
            is_move_valid = validate_pawn();
            if (is_move_valid)
                error = "";
            break;

        case 'r':
            error = "Invalid rook move";
            is_move_valid = validate_rook();
            if (is_move_valid)
                error = "";
            break;

        case 'b':
            error = "Invalid bishop move";
            is_move_valid = validate_bishop();
            if (is_move_valid)
                error = "";
            break;

        case 'n':
            error = "Invalid knight move";
            is_move_valid = validate_knight();
            if (is_move_valid)
                error = "";
            break;

        case 'q':
            error = "Invalid queen move";
            is_move_valid = validate_queen();
            if (is_move_valid)
                error = "";
            break;

        case 'k':
            error = "Invalid king move";
            is_move_valid = validate_king();
            if (is_move_valid)
                error = "";
            break;

        default:
            is_move_valid = false;
            break;
    }
    return is_move_valid;
}

void make_move () {


    board[y2][x2] = board[y1][x1];
    board[y1][x1] = ' ';

    turn %= 2;
    turn++;


}

void load_game() {

    ifstream fin("game.txt");
    string line;
    for (int i = 0; i < 8; i++) {
        getline(fin, line);
        for (int j = 0; j < 8; j++) {
            board[i][j] = line[j];
        }
    }
    fin.close();
}
void save_game() {
    ofstream fout("game.txt");
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            fout << board[i][j];
        }
        fout << endl;
    }
    fout.close();

}

int main()
{


    while (true) {
        system("cls");
        cout << error << endl;
        print_board();
        cout << "Player " << turn << " turn" << endl;
        char choice;
        do {
            cin >> choice;
            choice = tolower(choice);
        } while (choice != 'q' && choice != 'p' && choice != 'l' && choice != 's' && choice != 'h' && choice != 'd');

        switch (choice) {
            case 'p':
                break;

            case 'q':
                if (turn == 1)
                    cout << "Player 2 Won!" << endl;
                else
                    cout << "Player 1 Won!" << endl;
                return 0;
                break;

            case 'h':
                cout << "Game Drawn" << endl;
                return 0;
                break;

            case 'l':
                load_game();
                continue;
                break;

            case 's':
                save_game();
                cout << "Game Saved" << endl;
                continue;
                break;

            case 'd':
                continue;
                break;

        }


        get_input();

        if (check_move())
            make_move();
    }


    return 0;
}
