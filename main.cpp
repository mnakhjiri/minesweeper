#include "clui.h"
#include<iostream>
#include<stdlib.h>
#include<fstream>
#include<time.h>
#include<string.h>
#include<ctime>
#include<string>    
using namespace std;
//
void welcome();
void menuHandler();
void checkFile();
void printArrayStarter();
void printArrayUpdater();
void saveUserScore();
void saveGame();
int getScore();
void saveGameFile();
int getNumber(char* first_number);
void printArrayCompleted();
void copyArray(int dest[] , int from[] , int length);
void copyArrayTwod(int dest[][100] , int from[][100] , int length);
bool inBound(int number);
void game(int state);
void changeArray(int number , int to);
void makeCenter(const char* str , int row);
void makeCenterNormal(char* str , int row);
bool checkArray(int number , int to);
bool isBomb(int number);
bool inBoundNumbered(int i , int j , int count);
void checkwin();
void arrayToNumbered();
void loseGame();
bool isNumbered(int number);
int corToInt(int i , int j );
void man();
//
int flag_count = 0;
bool cheat_mode = false;
bool developer_mode = false;
int COUNT = 5;
int BOMB_COUNT = 4;
bool scored = false;
char difficulty = 'e';
int menu_number = 1;
int dif_number = 1;
int saved_games = 0;
bool footage = true;
int loadedGame=0;
bool save_reached_max = false;
bool isLoaded = false;
int loadmenu = 0;
int selected_house = 1;
// the main array that is shown to the user :
// -1 means the house is flaged , 0 means nothing has happend , 1 means the house is numberd , 2 means the house is neither flagged nor numbered
int array[100][100];
// the array that contains the bomb locations
int array_bombs[100] = {0};
// this array contains the number that each location has
int array_numbered[100][100] = {0};
struct player{
    char player_name[1000];
    int current_score = 0; 
    int max_score = 0;
};
struct save{
    char date[1000];
    int count;
    int bomb_count;
    int mainArray[100][100];
    int bombArray[100] = {0};
    int array_numbered[100][100] = {0};
    bool ended = false;
    bool won = false;
};
save saves[4];
player user;
player players[1000];
int number_of_players = 0;
int main(){
    init_clui();
    checkFile();
    //shows the manual and starts the program
    welcome();
}
// welcomes the user
void welcome(){
    clear_screen();
    // change_color_rgb(0, 10, 200);
    for(int i = 0 ; i <= get_window_rows() ; i++){
        for(int j = 0 ; j <= get_window_cols() ; j++){
            cursor_to_pos(i , j);

            change_background_color(BG_BLUE);
            cout<<" ";
        }  
    }
    change_color_rgb(0, 0, 0);
    makeCenter("WELCOME TO THE MINESWEEPER GAME" , get_window_rows()/2 );
    for(int i = 0 ; i < 5 ; i++){
        strcpy (user.player_name,"guest");
        // printf("Hello welcome to Minesweeper game\n\n\n");
        for(int j = 0 ; j <=i ; j++){
            string stuff(j+1 , '.');
            makeCenter(stuff.c_str() , get_window_rows()/2+1 );
        }
        delay(1000);
    }
    clear_screen();
    reset_color();
    man();
}
void goodbye(){
    clear_screen();
    change_color_rgb(200, 10, 0);
        printf("Have a nice day:)\n\n\n");
    delay(2000);
    reset_color();
    quit();
}
void changenameFile(){
    fstream file;
    file.open("leaderboard.txt" , ios::in);//read
    bool isFound = false;
    if(file.is_open()){
        string line;
        while(getline(file , line)){
            char userName[1000];
            int j = 0;
            for(int i = 0 ; line[i] != '-' ; i++){
                userName[i] = line[i];
                j = i;
            }
            j++;
            userName[j] = '\0';
            int score = getNumber(&line[j+1]);
            if(!strcmp(userName , user.player_name)){
                user.max_score = score;
                isFound = true;
            }
        }
        file.close();
    }
    if(isFound == false){
        user.max_score = 0;
        saveUserScore();
    }
}
void changename(){
    clear_screen();
    printf("Please enter your name: \n");
    cin>>user.player_name;
    saved_games = 0;
    loadmenu = 0;
    save_reached_max = false;
    changenameFile();
    reset_color();
    man();
}
void custom(){
    bool countcorrect = false;
    while(!countcorrect){
        clear_screen();
        printf("Please enter the number of rows/cols (it should be between 5 to 20) :");
        cin>>COUNT;
        if(COUNT>20 || COUNT <5){
            countcorrect = false;
        }else{
            countcorrect = true;
        }
    }
    bool bombcorrect = false;
    while(!bombcorrect){
        clear_screen();
        printf("Please enter the number of bombs: (it should be between 1  to rows * rows)");
        cin>>BOMB_COUNT;
        if(BOMB_COUNT>COUNT*COUNT || BOMB_COUNT <1){
            bombcorrect = false;
        }else{
            bombcorrect = true;
        }
    }
    game(0);
}
char* changestr( const char* str){
    char* result = new char[100];
    strcpy(result , str);
    return result;
}
void setDifficulty(){
    clear_screen();
    for(int i = 3 ; i <= 3 + number_of_players*3 + 2  ; i++){
            for(int j = get_window_cols()/2 - get_window_cols()/8 ; j <= get_window_cols()/2 + get_window_cols()/8 ; j++){
                cursor_to_pos(i , j);
                change_background_color(BG_GREEN);
                printf(" ");
            }
    }
    
    change_background_color(BG_GREEN);
    change_color_rgb(0,0,0);
    if(dif_number == 1){
        change_color_rgb(0, 200, 0);
    }
    char* temp = changestr("Easy");
    makeCenterNormal(temp , 5 );
    delete[] temp;
    reset_color();
    change_background_color(BG_GREEN);
    change_color_rgb(0,0,0);
    if(dif_number == 2){
        change_color_rgb(0, 200, 0);
    }
    temp = changestr("Medium");
    makeCenterNormal(temp , 8);
    delete[] temp;
    reset_color();
    change_background_color(BG_GREEN);
    change_color_rgb(0,0,0);
    if(dif_number == 3){
        change_color_rgb(0, 200, 0);
    }
    temp = changestr("Hard");
    makeCenterNormal(temp , 11);
    delete[] temp;
    reset_color();
    change_background_color(BG_GREEN);
    change_color_rgb(0,0,0);
    if(dif_number == 4){
        change_color_rgb(0, 200, 0);
    }
    temp = changestr("Custom");
    makeCenterNormal(temp , 14);
    delete[] temp;
    reset_color();
    char input = getch();
    if(input == 's' && dif_number < 4){
        dif_number++;
        setDifficulty();
    }else if(input == 'w' && dif_number > 1){
        dif_number--;
        setDifficulty();
    }else if(input == 13){
        selected_house = 1;
        COUNT = 5;
        BOMB_COUNT = 4;
        if(dif_number == 1){
            difficulty = 'e';
            COUNT = 5;
            BOMB_COUNT = 4;
        }
        if(dif_number == 2){
            difficulty = 'm';
            COUNT = 12;
            BOMB_COUNT = 28;
        }
        if(dif_number == 3){
            difficulty = 'h';
            COUNT = 20;
            BOMB_COUNT = 96;
        }
        if(dif_number == 4){
            clear_screen();
            custom();
        }
        dif_number = 1;
        if(saved_games == 4){
            saved_games = 0;
            save_reached_max = true;
        }
        saved_games++;
        if(save_reached_max == true){
            saves[saved_games-1].ended = false;    
        }
        isLoaded = false;
        game(0);
    }else{
     setDifficulty();   
    }
        
}
void swap(player& a , player& b){
    player temp = a;
    a = b;
    b = temp;
}
void playerSort(){
    for(int i = 0 ; i < number_of_players-1 ; i++){
        for(int j = i ; j < number_of_players ; j++){
            if(players[i].max_score < players[j].max_score){
                swap(players[i] , players[j]);
            }
        }
    }
}
const char* str(int number){
    int length = 0;
    int revnumber = 0;
    for(int i = number ; i>0 ; i/=10){
        length++;
    }
    char* temp = new char[length+1];
    for(int i = 0 ; i < length ; i++){
        revnumber = revnumber*10 + number%10;
        number/=10;
    }
    int index = 0;
    for(int i = 0 ; i < length ; i++){
        temp[index] = revnumber%10 + '0';
        revnumber/=10;
        index++;
    }
    temp[length] = '\0';
    const char* result = temp;
    return result;
}
void leaderboard(){
    clear_screen();
    playerSort();
        
    if(number_of_players == 0){
        change_color_rgb(32, 52, 255);
        char totalline[1000];
        strcpy(totalline , "There is nothing to show:)" );
        makeCenterNormal(totalline , 8);
        reset_color();
    }else{
        for(int i = 3 ; i <= 3 + number_of_players*3 + 2  ; i++){
            for(int j = get_window_cols()/2 - get_window_cols()/8 ; j <= get_window_cols()/2 + get_window_cols()/8 ; j++){
                cursor_to_pos(i , j);
                change_background_color(BG_GREEN);
                printf(" ");
            }
        }
    }
    for(int i = 0 ; i < number_of_players ; i++){
        // printf("%s : %d\n\n" , players[i].player_name , players[i].max_score);
        char totalline[1000];
        strcpy(totalline , str(i+1));
        strcat(totalline , ") ");
        strcat(totalline , players[i].player_name);
        strcat(totalline , " : ");
        if(players[i].max_score == 0){
            strcat(totalline , "0");
        }else{
            const char* temp = str(players[i].max_score);
            strcat(totalline , temp);
            delete[] temp;
        }
        change_background_color(BG_GREEN);
        change_color_rgb(0,0,0);
        makeCenterNormal(totalline , 5 + i*3);
        reset_color();
    }
    // printf("press esc key to get back to the menu\n");
    char input = getch();
    if(input == 27){
        man();
    }else{
        leaderboard();
    }
}
void showCompletedGame(bool won){
    if(won == true){
        checkwin();
    }else{
        loseGame();
    }
}
void loadGame(){
    //clear_screen();
    static bool bgLoadedsub = false;
    int  spaceBetween = 3;
    if(bgLoadedsub == false){
        for(int i = get_window_rows()/2-spaceBetween*4 ; i <=get_window_rows()/2 + spaceBetween*4  ; i++){
            for(int j = get_window_cols()/2 - get_window_cols()/4 ; j <= get_window_cols()/2 + get_window_cols()/4 ; j++){
                cursor_to_pos(i , j);
                change_background_color(BG_GREEN);
                printf(" ");
            }
        }
        bgLoadedsub = true;
    }
    if(save_reached_max == false){
        for(int i = 0 ; i < saved_games ; i++){
            change_background_color(BG_GREEN);
            change_color_rgb(0, 0, 0);
            if(loadmenu == i){
                change_color_rgb(200, 200, 200);
            }
            makeCenterNormal(saves[i].date , get_window_rows()/2-spaceBetween*3 +spaceBetween*i);
            reset_color();
        }
        if(saved_games == 0){
            char str[1000];
            strcpy(str , "There is nothing to show:)");
            makeCenterNormal(str , get_window_rows()/2-spaceBetween*3);
        }
        // printf("please enter esc key to get back to the menu");
        char input = getch();
        if(input == 'w' && loadmenu>0){
            loadmenu--;
            loadGame();
        } else if(input == 's' && loadmenu<saved_games-1){
            loadmenu++;
            loadGame();
        }else if(input == 13){
            if(saved_games == 0){
                loadGame();
            }
            copyArrayTwod( array , saves[loadmenu].mainArray , 100);
            copyArray( array_bombs , saves[loadmenu].bombArray , 100);
            copyArrayTwod( array_numbered , saves[loadmenu].array_numbered , 100);
            COUNT = saves[loadmenu].count;
            BOMB_COUNT = saves[loadmenu].bomb_count;
            loadedGame = loadmenu;
            isLoaded = true;
            if(saves[loadmenu].ended == true){
                if(saves[loadmenu].won==true){
                    bgLoadedsub = false;
                    showCompletedGame(true);
                }else{
                    bgLoadedsub = false;
                    showCompletedGame(false);
                }
            }else{
                bgLoadedsub = false;
                selected_house = 1;
                game(1);
            }
        }else if(input == 27){
            bgLoadedsub = false;
            man();
        }else{        
            loadGame();
        }
    }else{
        for(int i = 0 ; i < 4 ; i++){
            
            change_background_color(BG_GREEN);
            change_color_rgb(0, 0, 0);
            if(loadmenu == i){
                change_color_rgb(200, 200, 200);
            }
            makeCenterNormal(saves[i].date , get_window_rows()/2-spaceBetween*3 +spaceBetween*i);
            reset_color();
        }
        char input = getch();
        if(input == 'w' && loadmenu>0){
            loadmenu--;
            loadGame();
        } else if(input == 's' && loadmenu<3){
            loadmenu++;
            loadGame();
        }else if(input == 13){
            copyArrayTwod( array , saves[loadmenu].mainArray , 100);
            copyArray( array_bombs , saves[loadmenu].bombArray , 100);
            copyArrayTwod( array_numbered , saves[loadmenu].array_numbered , 100);
            loadedGame = loadmenu;
            isLoaded = true;
            COUNT = saves[loadmenu].count;
            BOMB_COUNT = saves[loadmenu].bomb_count;
            if(saves[loadmenu].ended == true){
                if(saves[loadmenu].won==true){
                    bgLoadedsub = false;
                    showCompletedGame(true);
                }else{
                    bgLoadedsub = false;
                    showCompletedGame(false);
                }
            }else{
                bgLoadedsub = false;
                selected_house = 1;
                game(1);
            }
        } else if(input == 27){
            bgLoadedsub = false;
            man();
        }else{
            loadGame();
        }

    }
    
    
}
void man(){
    static bool bgLoaded = false;
    if(bgLoaded == false){
        for(int i = 0 ; i <= get_window_rows() ; i++){
            for(int j = 0 ; j <= get_window_cols() ; j++){
                cursor_to_pos(i , j);

                change_background_color(BG_BLUE);
                printf(" ");
            }  
        }
        bgLoaded = true;
    }
    
    flag_count = 0;
    int spaceBetween = 3;
    static bool bgLoadedsub = false;
    if(bgLoadedsub == false){
        for(int i = get_window_rows()/2-spaceBetween*4 ; i <=get_window_rows()/2 + spaceBetween*4  ; i++){
            for(int j = get_window_cols()/2 - get_window_cols()/4 ; j <= get_window_cols()/2 + get_window_cols()/4 ; j++){
                cursor_to_pos(i , j);
                change_background_color(BG_BLUE);
                printf(" ");
            }
        }
        bgLoadedsub = true;
    }
    change_background_color(BG_BLUE);
    makeCenter(  "MENU PAGE" , get_window_rows()/2-spaceBetween*3);
    reset_color();
    char totalline[1000];
    strcpy(totalline , "NAME : ");
    strcat(totalline , user.player_name);
    change_background_color(BG_BLUE);
    makeCenterNormal(totalline , get_window_rows()/2-spaceBetween*2);
    // printf("Name : %s\n\n" , user.player_name);
    if(menu_number == 1){
        change_color_rgb(0, 200, 0);
    }
    change_background_color(BG_BLUE);
    makeCenter("NEW GAME" , get_window_rows()/2-spaceBetween);
    // printf("New Game\n\n");
    reset_color();
    if(menu_number == 2){
        change_color_rgb(0, 200, 0);
    }
    change_background_color(BG_BLUE);
    makeCenter("LOAD GAME" , get_window_rows()/2);
    // printf("Load Game\n\n");
    reset_color();
    if(menu_number == 3){
        change_color_rgb(0, 200, 0);
    }
    change_background_color(BG_BLUE);
    makeCenter("CHANGE NAME" , get_window_rows()/2 + spaceBetween);
    // printf("Change Name\n\n");
    reset_color();
    if(menu_number == 4){
        change_color_rgb(0, 200, 0);
    }
    change_background_color(BG_BLUE);
    makeCenter("LEADERBOARD" , get_window_rows()/2 + spaceBetween*2);
    // printf("Leaderboard\n\n");
    reset_color();
    if(menu_number == 5){
        change_color_rgb(0, 200, 0);
    }
    change_background_color(BG_BLUE);
    makeCenter("QUIT" , get_window_rows()/2 + spaceBetween*3);
    // printf("Quit\n\n");
    reset_color();
    char input = getch();
    if(input == 's' && menu_number < 5){
        menu_number++;
        man();
    }
    if(input == 'w' && menu_number > 1){
        menu_number--;
        man();
    }
    if(input == 13){
        bgLoadedsub = false;
        menuHandler();
    }
    man();
}

void menuHandler(){
    if(menu_number == 1){
        menu_number = 1 ;
        setDifficulty();
    }
    if(menu_number == 2){
        menu_number = 1 ;
        loadGame();
    }
    if(menu_number == 5){
        menu_number = 1 ;
        goodbye();
    }
    if(menu_number == 3){
        menu_number = 1;
        changename();
    }
    if(menu_number == 4){
        menu_number = 1;
        leaderboard();
    }
}
void gameKeyboard(){
    char input = getch();

    if(input == 'w'){
        if(selected_house - COUNT > 0){
            selected_house = selected_house - COUNT;
        }
    }
    if(input == 's'){
        if(selected_house + COUNT <= COUNT*COUNT){
            selected_house = selected_house + COUNT;
        }
    }
    if(input == 'd'){
        if(selected_house + 1 <= COUNT*COUNT){
            selected_house = selected_house + 1;
        }
    }
    if(input == 'a'){
        if(selected_house - 1 > 0){
            selected_house = selected_house - 1;
        }
    }
    if(input == 'f'){
        if(flag_count<BOMB_COUNT && flag_count>=0 && checkArray(selected_house , -1)==false && checkArray(selected_house , 1)==false && checkArray(selected_house , 2)==false ){
                changeArray(selected_house,-1);
                flag_count++;
        }else if(checkArray(selected_house , -1)==true){
                changeArray(selected_house,0); 
                flag_count--;
        }
    }
    if(input == 13){
         if(checkArray(selected_house , -1) && inBound(selected_house)){
            
        }else if(isBomb(selected_house)){
            play_beep();
            scored = false;
            loseGame();
        }else if(inBound(selected_house)){
            if(isNumbered(selected_house)){
                // change the house position to numbered
                changeArray(selected_house , 1);
                game(1);
            }else{
                // change the house position to unnumbered
                changeArray(selected_house,2);
                game(1);
            }
        }else{
        }
    }
    if(input == 'o'){
        saveGame();
        man();
    }
    if(input == 'q'){
        man();
    }
    game(1);
}
int getNumber(char* first_number){
    int result = 0;
    while(*first_number <='9' && *first_number >= '0'){
        result = result*10 + (*first_number - '0');
        first_number++;
    };
    return result;
}
void checkFile(){
    fstream file;
    file.open("leaderboard.txt" , ios::in);//read
    
    if(file.is_open()){
        string line;
        while(getline(file , line)){
            player gamer;
            char userName[1000];
            int j = 0;
            for(int i = 0 ; line[i] != '-' ; i++){
                userName[i] = line[i];
                j = i;
            }
            j++;
            userName[j] = '\0';
            int score = getNumber(&line[j+1]);
            strcpy(gamer.player_name , userName);
            gamer.max_score = score;
            players[number_of_players] = gamer;
            number_of_players++;
        }
        file.close();
    }
}
// the main function that controls the game mode
void game(int state = 0){
    //prints the first state of the game
    if(state == 0){
        clear_screen();
        printArrayStarter();
        footage = true;
        gameKeyboard();
    }
    // prints the updated state of the game
    if(state == 1){
        scored = false;
        checkwin();// checks if the user has one the game in each updated state
        clear_screen();
        printArrayUpdater();
        gameKeyboard();
    }

}
// closes the game after the user has lost the game
void loseGame(){
    
    clear_screen();
    saveGame();
    if(isLoaded == true){
        saves[loadmenu].ended = true;
        saves[loadmenu].won = false;
    }else{
        saves[saved_games-1].ended = true;
        saves[saved_games-1].won = false;
    }
    change_color_rgb(250 , 0 , 0);
    printf("You have lost the game , press Enter key to continue\n\n");
    reset_color();
    printf("Your score is : %d\n\n" , getScore());
    if(scored == false){
        user.max_score += getScore();
        saveUserScore();
        scored = true;
    }
    // cout<<user.max_score;
    printArrayCompleted();
    char input = getch();
    if(input == 13){
        reset_color();
        man();
    }else{
        loseGame();
    }
}

void checkwin(){
    // checks if all houses are selected or flagged
    for(int i = 0 ; i < COUNT ; i++){
        for(int j = 0 ; j < COUNT ; j++){
            if( array[i][j] == 0 && !isBomb(corToInt(i , j)) ){
                return;
            }
        }
    }
    saveGame();
    if(isLoaded == true){
        saves[loadmenu].ended = true;
        saves[loadmenu].won = true;
    }else{
        saves[saved_games-1].ended = true;
        saves[saved_games-1].won = true;
    }
    // waiting screen after the user has won the game :)
    clear_screen();
    change_color_rgb(0 , 250 , 0);
    printf("You have won the game , press Enter key to continue\n\n");
    reset_color();
    printf("Your score is : %d" , getScore());
    if(scored == false){
        user.max_score += COUNT*COUNT;
        saveUserScore();
        scored = true;
    } 
    char input = getch();
    if(input == 13){
        reset_color();
        man();
    }else{
        checkwin();
    }
}
int getScore(){
    int result = 0;
    for(int i = 0 ; i < COUNT ; i ++){
        for(int j = 0 ; j < COUNT ; j++){
            if(array[i][j] == 1 || array[i][j] == 2){
                result++;
            }
        }
    }
    return result;
}
void saveUserScore(){
    fstream file;
    bool isFound = false;
    file.open("leaderboard.txt" , ios::in);
    if(file.is_open()){
        string line;
        while(getline(file , line)){
            player gamer;
            char userName[1000];
            int j = 0;
            for(int i = 0 ; line[i] != '-' ; i++){
                userName[i] = line[i];
                j = i;
            }
            j++;
            userName[j] = '\0';
            if(!strcmp(userName , user.player_name)){
                isFound = true;
            }
        }
        file.close();
    }
    file.open("leaderboard.txt" , ios::out);
    if(file.is_open()){
        if(isFound == false){
            file<<user.player_name;
            file<<'-';
            file <<to_string(user.max_score) + '\n';
            for(int i = 0 ; i < number_of_players ; i++){
                file<<players[i].player_name;
                file<<'-';
                file <<to_string(players[i].max_score) + '\n';
            }
            players[number_of_players] = user; 
            number_of_players++;
        }else{
            for(int i = 0 ; i < number_of_players ; i++){
                if(!strcmp(players[i].player_name , user.player_name)){
                    players[i].max_score = user.max_score;
                }
                file << players[i].player_name;
                file<<'-';
                file<<to_string(players[i].max_score) + '\n';
            }
            // checkFile();
        }
        file.close();
    }
}
// start of the game and creating the array
void printArrayStarter(){
// create random bombs
    srand(time(NULL)); 
    int bomb_counter = 0;
    while(bomb_counter <BOMB_COUNT){
        int bomb_test = rand()%(COUNT*COUNT) +1;
        bool bomb_found = false;
        for(int i = 0 ; i < BOMB_COUNT; i++){
            if(array_bombs[i] == bomb_test){
                bomb_found = true;
            }
        }
        if(bomb_found == true){
            continue;
        }
        array_bombs[bomb_counter] = bomb_test;
        bomb_counter++;
    }
    arrayToNumbered();
    int pos = 1;
        for(int i = 0 ; i < COUNT ; i++){
                for(int j = 0 ; j < COUNT ; j++){

                    // this comment below is a code to check if the bombs are random and corectly counted and the cols are  numbered
                    if(cheat_mode == true){
                        if(isBomb(pos)){
                        change_color_rgb(200, 0, 0);
                        printf("X ");
                        reset_color();
                        }else if(array_numbered[i][j]!=0){
                            change_color_rgb(0, 150, 0);
                            printf("%d " , array_numbered[i][j]);
                            reset_color();
                        }
                        else{
                            change_color_rgb(135, 135, 135);
                            printf("O ");
                            reset_color();
                        }
                        
                    }else{
                        if(selected_house == pos){
                            change_color_rgb(0, 135, 0);
                            printf("# ");
                            reset_color();
                        }else{
                            change_color_rgb(135, 135, 135);
                            printf("# ");
                            reset_color();
                        }
                        
                        array[i][j] = 0;    
                    }
                    
                    pos++;
                }
                
                printf("\n");
        }
        printf("\nscore : %d\n\n" , getScore() );
            user.current_score = getScore();
            
        printf("remaining flags : %d" , BOMB_COUNT - flag_count);
        cheat_mode = false;
}
void copyArrayTwod(int dest[][100] , int from[][100] , int length){    
        for(int i = 0 ; i < length ; i++){
            for(int j = 0 ; j < length ; j++){
                dest[i][j] = from[i][j];
                
                
            }
        }
}
void copyArray(int dest[] , int from[] , int length){
    for(int i = 0 ; i < length ; i++){
        dest[i]= from[i];
    }
}
void saveGame(){
    
    // saveGameFile();
    if(isLoaded == true){
        
        copyArrayTwod(saves[loadmenu].mainArray , array , 100);
        copyArray(saves[loadmenu].bombArray , array_bombs , 100);
        copyArrayTwod(saves[loadmenu].array_numbered , array_numbered , 100);    
    }else{
        time_t now = time(0);
        char* dt = ctime(&now);
        strcpy(saves[saved_games-1].date , dt);
        saves[saved_games-1].count = COUNT;
        saves[saved_games-1].bomb_count = BOMB_COUNT;
        copyArrayTwod(saves[saved_games-1].mainArray , array , 100);
        copyArray(saves[saved_games-1].bombArray , array_bombs , 100);
        copyArrayTwod(saves[saved_games-1].array_numbered , array_numbered , 100);
    }
    
}
// updates the array and prints it
void printArrayUpdater(){
int pos = 1;
        
        for(int i = 0 ; i < COUNT ; i++){
                for(int j = 0 ; j < COUNT ; j++){
                    reset_color();
                    if(array[i][j] == -1){
                        if(selected_house == pos){
                            change_color_rgb(0, 135, 0);
                            printf("F ");
                            reset_color();
                        }else{
                            change_color_rgb(135, 0, 0);
                            printf("F ");
                            reset_color();
                        }
                    }
                    if(array[i][j] == 0){
                        if(selected_house == pos){
                            change_color_rgb(0, 135, 0);
                            printf("# ");
                            reset_color();
                        }else{
                            change_color_rgb(135, 135, 135);
                            printf("# ");
                            reset_color();
                        }
                    }
                    if(array[i][j] == 1){
                        if(selected_house == pos){
                            change_color_rgb(0, 135, 0);
                            printf("%d " , array_numbered[i][j]);
                            reset_color();
                        }else{
                            change_color_rgb(135, 135, 135);
                            printf("%d " , array_numbered[i][j]);
                            reset_color();
                        }
                    }
                    if(array[i][j] == 2){
                        if(selected_house == pos){
                            change_color_rgb(0, 135, 0);
                            printf("- ");
                            reset_color();
                        }else{
                            change_color_rgb(135, 135, 135);
                            printf("- ");
                            reset_color();
                        }
                    }
                    pos++;
                }
                printf("\n");
        }
        
        printf("\nscore : %d\n\n" , getScore());
        user.current_score = getScore();
        // saveGame();
        printf("remaining flags : %d" , BOMB_COUNT - flag_count);
        
        reset_color();
}
// prints the whole array showing bombs and etc
void printArrayCompleted(){
    int pos = 1;
    reset_color();
    for(int i = 0 ; i < COUNT ; i++){
        for(int j = 0 ; j < COUNT ; j++){
            if(isBomb(pos)){
                change_color_rgb(200, 0, 0);
                printf("X ");
                reset_color();
            }else if(array_numbered[i][j]!=0){
                change_color_rgb(102, 250, 142);
                printf("%d " , array_numbered[i][j]);
                reset_color();
            }
            else{
                change_color_rgb(135, 135, 135);
                printf("O ");
                reset_color();
            }
            pos++;
        }
        printf("\n");
    }
}
// checks if the input is inbound
bool inBound(int number){
    if(number >= 1 && number <= COUNT*COUNT){
        return true;
    }else{
        return false;
    }
}
// changing the array status if the number has selected it
void changeArray(int number , int to){
    int counter = 1;
    // a recursive function to select all the unnumbered houses
    if(to == 2){
        int pos=1;
        for(int i = 0 ; i < COUNT ; i++){
            for(int j = 0 ; j < COUNT ; j ++){
                int bomb_counter = 0;
                if(pos == number){
                    array[i][j] = to;
                    for(int changei = -1 ; changei <= 1 ; changei++ ){
                        for(int changej = -1 ; changej <=1 ; changej++){
                            if(changei != 0 || changej != 0){
                                if(inBoundNumbered(i + changei , j + changej ,COUNT)){
                                    if(!isNumbered(corToInt(i + changei , j + changej)) && !isBomb(corToInt(i + changei , j + changej)) && array[i+changei][j + changej] !=2){
                                        changeArray(corToInt(i + changei , j + changej) , 2);
                                    }
                                    if(isNumbered(corToInt(i + changei , j + changej))){
                                        changeArray(corToInt(i + changei , j + changej) , 1);
                                    }
                                }
                            }
                        }
                    }
                }
                pos++; 
            }
        }  
    }else{
        for(int i = 0 ; i < COUNT ; i++){
        for(int j = 0 ; j < COUNT ; j++){
            if( counter == number){
                array[i][j] = to;
            }
            counter++;
        }
        }
    }
}
// checks if the number has the given value or not
bool checkArray(int number , int to){
    int counter = 1;
    for(int i = 0 ; i < COUNT ; i++){
        for(int j = 0 ; j < COUNT ; j++){
            if( counter == number){
                if(array[i][j] == to){
                    return true;
                }else{
                    return false;
                }
            }
            counter++;
        }
    }
}

bool isBomb(int number){
    int counter = 1;
    for(int i = 0 ; i < BOMB_COUNT ; i++){
        if(number == array_bombs[i]){
            return true;
        }
    }
    return false;
}
// changing the coordinates to number
int corToInt(int i , int j ){
    int pos = 1;
    for(int x = 0 ; x < COUNT ; x++ ){
        for(int y = 0 ; y < COUNT ; y++){
            if(x == i && y == j){
                return pos;
            }
            pos++;
        }
    }
}
// numbering the bombs
bool inBoundNumbered(int i , int j , int count){
    if(i >= 0 && i <= count-1 && j >= 0 && j <= count-1){
        return true;
    }else{
        return false;
    }
}
// completing the array_numbered array
void arrayToNumbered(){
    for(int i = 0 ; i < COUNT ; i++){
        for(int j = 0 ; j < COUNT ; j ++){
            int bomb_counter = 0;
            for(int changei = -1 ; changei <= 1 ; changei++ ){
                for(int changej = -1 ; changej <=1 ; changej++){
                    if(changei != 0 || changej != 0){
                        if(inBoundNumbered(i + changei , j + changej ,COUNT)){
                           if(isBomb(corToInt(i + changei , j + changej))){
                               bomb_counter++;
                           }
                        }
                    }
                }
            }   
            array_numbered[i][j] = bomb_counter;
        }
    }
}
// checks if the array location is numbered
bool isNumbered(int number){
    for(int i = 0 ; i < COUNT ; i++){
        for(int j = 0 ; j < COUNT ; j++){
            if(corToInt(i , j) == number){
                if(array_numbered[i][j]>0){
                    return true;
                }
            }
        }
    }
    return false;

}
void makeCenter(const char* str , int row){
    int length = strlen(str);
    int cols = get_window_cols();
    cols = cols/2 + 1;
    cursor_to_pos(row , cols-length/2);
    cout<<str;
}
void makeCenterNormal(char* str , int row){
    int length = strlen(str);
    int cols = get_window_cols();
    cols = cols/2 + 1;
    cursor_to_pos(row , cols-length/2);
    cout<<str;
}