//g++ doging.cpp -o doging.exe && start doging.exe
#include "colors.cpp"
//config const͡s {
const int MAX_SPEED = 3;
const int KASHTURA_MAX_HEALTH = 3;
const char SNOWBALL_CH = '@';
const COLORS SNOWBALL_COL = WHITE;
const char KASHTURA_CH = '@';
const COLORS KASHTURA_COL = BROWN;
const char GENADI_CH = '@';
const COLORS GENADI_COL = BLUE;
const COLORS SCORE_COL = WHITE;
const int GRND_LVL_PRCNT = 15;
const COLORS GRND_COL = GREEN;
const char GRND_CH = '@';
const char LIFE_CH = '@';
const COLORS LIFE_COL = RED;
const int GENADI_MAX_HEALTH = 3;
const COLORS DEDMSG_COL = BLACK;
//} config const͡s
const int GRND_LVL = (MAP_ROWS * (100-GRND_LVL_PRCNT))/100;
struct Snowball {
    int r, c;
    int oldR, oldC;
    char ch;
    COLORS col;
    int spd;
    Snowball(){
        r = 0;
        c = rand()%MAP_COLS;
        spd = (rand()%MAX_SPEED)+1;
    }
    void move(){
        oldR = r;
        oldC = c;
        r += spd;
        r %= MAP_ROWS;
    }
    void show(){
        draw_char(' ',oldR, oldC, BKG_COL, BKG_COL);
        draw_char(ch, r, c, col, BKG_COL);
    }
};
struct Kashtura {
    int r,c;
    char ch;
    COLORS col;
    int health;
    Kashtura(){
        health = 0;        
    }
    void place(int ir, int ic){
        draw_char(' ',r,c,BKG_COL,BKG_COL);
        health = KASHTURA_MAX_HEALTH;
        r = ir;
        c = ic;
    }
    void show(){
        draw_char(ch,r,c,col,BKG_COL);
    }
    void protect(vector<Snowball> &snowballs){
        for(int s=0;s<snowballs.size();s++){
            if(snowballs[s].r==r && snowballs[s].c==c){
                draw_char(' ',snowballs[s].r,snowballs[s].c,BKG_COL,BKG_COL);           // TOVA CHE SI MURTAV NE OZNACHAVA CHE NE SI JIV...
                snowballs.erase(snowballs.begin()+s);
                s--;
                health--;
            }
        }
        if(health<=0){
            draw_char(' ',r,c,BKG_COL,BKG_COL);   
        }
    }
};
struct ControlsState{
    bool left;
    bool right;
    bool jump;
    bool place;
    void set(bool il, bool ir, bool ij, bool ip){
        left = il;
        right = ir;
        jump = ij;
        place = ip;
    }
};
struct Controls{
    char left;
    char right;
    char jump;
    char place;
    ControlsState state;
    void set(char il, char ir, char ij, char ip){
        left = il;
        right = ir;
        jump = ij;
        place = ip;
    }
    void getState(){
        bool l,r,j,p;
        l = GetAsyncKeyState(left);
        r = GetAsyncKeyState(right);
        j = GetAsyncKeyState(jump);
        p = GetAsyncKeyState(place);
        state.set(l,r,j,p);
    }
};
struct Genadi{
    char ch;
    COLORS col;
    int r, c;
    int score;
    int health = GENADI_MAX_HEALTH;
    int oldR, oldC;
    Controls cont;
    Kashtura k;
    int ks;
    Genadi(){score=0;}
    void getState(){
        cont.getState();
    }
    void bonk(vector<Snowball> &sballs){
        for(int s = 0; s < sballs.size(); s++){
            if(sballs[s].r == r && sballs[s].c == c){
                health--;
                draw_char(' ',sballs[s].r,sballs[s].c,BKG_COL,BKG_COL);//TOVA CHE SI MURTAV NE OZNACHAVA CHE NE SI JIV... AZ NE BQGAM AZ PROSTO TICHAM
                sballs.erase(sballs.begin()+s);
                s--;
            }
        }
    }
    void control(){
        oldC = c;
        oldR = r;
        if(cont.state.left){
            c--;
        }
        if(cont.state.right){
            c++;
        }
        if(cont.state.place){
            if((k.health <= 0) && (ks > 0)){
                k.r = r-1;
                k.c = c;
                k.health = KASHTURA_MAX_HEALTH;
                ks--;
            }
        }
    }
    void show(){
        draw_char(' ',oldR, oldC, BKG_COL, BKG_COL);
        draw_char(ch,r,c,col,BKG_COL);
        if(k.health > 0){
            k.show();
        }else{
            draw_char(' ',k.r,k.c,BKG_COL,BKG_COL);
        }
    }
};
void draw_string(string label, int y, int x, COLORS foreground_color, COLORS background_color){
    for(int i = 0; label[i] != 0; i++){
        draw_char(label[i], y, x + i, foreground_color, background_color);
    }
}
void draw_number(int value, int y, int x, COLORS foreground_color, COLORS background_color) {
    int br = 0;
    if (value < 0) {
        draw_char('-', y, x, foreground_color, background_color);
        value *= -1;
    }else {
        draw_char('+', y, x, foreground_color, background_color);
    }
    if (value == 0) {
        draw_char('0', y, x + 10, foreground_color, background_color);
        return;
    }
    for (int i = value; i > 0; i = i / 10) {
        draw_char(i%10 + '0', y, x + 10 - br, foreground_color, background_color);
        br++;
    }
    draw_char(' ', y, x + 10 - br, foreground_color, background_color);
}
void draw_pair(string label, int value, int y, int x, COLORS foreground_color, COLORS background_color){
    draw_string(label, y, x, foreground_color, background_color);
    draw_number(value, y, x + label.length(), foreground_color, background_color);
}
void kill_snowballs(vector<Snowball> &snowballs ){
    for(int s=0; s <snowballs.size();s++){
        if(snowballs[s].r>=GRND_LVL){
            draw_char(GRND_CH,snowballs[s].r,snowballs[s].c,GRND_COL,BKG_COL);
            snowballs.erase(snowballs.begin()+s);
            s--;
        }        
    }
}
void draw_ground() {
    for(int r = GRND_LVL; r < MAP_ROWS; r++) {
        for(int c= 0; c < MAP_COLS; c++ ) {
           draw_char(GRND_CH , r, c, GRND_COL, BKG_COL);

        }

    }
}

int main() {
    vector<Snowball> snowballs;
    Snowball sb;
    set_background();
    draw_ground();
    Genadi g;
    g.c = MAP_COLS/2;
    g.r = GRND_LVL-1;
    g.ch = GENADI_CH;
    g.col = GENADI_COL;
    g.cont.set('A','D',VK_SPACE,'K');
    g.k.ch = KASHTURA_CH;
    g.k.col = KASHTURA_COL;
    g.ks = 10;
    int br=0;
    while(g.health > 0){
        if(br % 5 == 0){
            g.score++;
            draw_pair("score: ", g.score, MAP_ROWS+2, MAP_COLS/2, SCORE_COL, BKG_COL);
        }
        g.bonk(snowballs);
        string lifes = "lifes: ";
        for(int h = 0; h < g.health; h++){
            lifes += LIFE_CH;
        }
        lifes += ' ';
        draw_string(lifes, MAP_ROWS+2, 0, LIFE_COL, BKG_COL);
        if(br % 3 == 0) { 
                for(int j=0; j<6; j++){
                    sb.r = 1;
                    sb.c = rand() % MAP_COLS;
                    sb.spd = 1;
                    sb.ch = '*';
                    sb.col = SNOWBALL_COL; 
                    snowballs.push_back(sb);
                } 
        }
        g.getState();
        g.control();
        g.show();
        for(int i=0; i<snowballs.size(); i++){
            snowballs[i].move();
            snowballs[i].show();
        }
        g.k.protect(snowballs);
        kill_snowballs(snowballs);
        Sleep(100);
        br++;
    }
    draw_pair("DED, score: ",g.score,MAP_ROWS/2,MAP_COLS/2-10,DEDMSG_COL,BKG_COL);
    system("pause > nul");
    return 0;
}
