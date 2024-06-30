#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <string>
#include "tile.h"
#include "board.h"
using namespace std;

void startWindow(sf::Font &font, int &cols, int &rows, int &mines);
void gameWindow(sf::Font &font, int &cols, int &rows, int &mines, string &name);
void leaderboardWindow(sf::Font &font, int &cols, int &rows);

void setText(sf::Text &text, float x, float y){
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,
                   textRect.top  + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(x, y));
}

void startWindow(sf::Font &font, int &cols, int &rows, int &mines) {
    int width = 800;
    int height = 600;
    sf::RenderWindow window(sf::VideoMode(width, height), "Name Insertion", sf::Style::Close);
    window.clear(sf::Color::Blue);

    sf::Text title, description, nameInput;
    title.setFont(font);
    description.setFont(font);
    nameInput.setFont(font);

    title.setString("WELCOME TO MINESWEEPER!");
    title.setFillColor(sf::Color::White);
    title.setStyle(sf::Text::Bold | sf::Text::Underlined);
    title.setCharacterSize(24);
    setText(title, (width / 2), (height / 2) - 150);
    window.draw(title);

    description.setString("Enter your name:");
    description.setFillColor(sf::Color::White);
    description.setStyle(sf::Text::Bold);
    description.setCharacterSize(20);
    setText(description, (width / 2), (height / 2) - 75);
    window.draw(description);

    string outString = "|";
    string regString;
    nameInput.setString(outString);
    nameInput.setFillColor(sf::Color::Yellow);
    nameInput.setStyle(sf::Text::Bold);
    nameInput.setCharacterSize(18);
    setText(nameInput, (width / 2), (height / 2) - 45);
    window.draw(nameInput);

    window.display();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::TextEntered) {
                char letter = static_cast<char>(event.text.unicode);
                if (regString.size() == 0) {
                    letter = toupper(letter);
                } else if (regString.size() >= 1) {
                    letter = tolower(letter);
                }
                if (isalpha(letter) && regString.size() < 10) {
                    regString += letter;
                    outString = regString + "|";
                    nameInput.setString(outString);
                    setText(nameInput, (width / 2), (height / 2) - 45);
                }
            }
            else if (event.type == sf::Event::KeyPressed && regString.size() > 0) {
                if (event.text.unicode == sf::Keyboard::Key::BackSpace) {
                    if (regString.size() > 0) {
                        regString.pop_back();
                        outString = regString + "|";
                        nameInput.setString(outString);
                        setText(nameInput, (width / 2), (height / 2) - 45);
                    }
                } else if (event.text.unicode == sf::Keyboard::Key::Enter) {
                    window.close();
                    gameWindow(font, cols, rows, mines, regString);
                }
            }
            //Updating Page
            window.clear(sf::Color::Blue);
            window.draw(title);
            window.draw(description);
            window.draw(nameInput);
            window.display();
        }

    }
}

void gameWindow(sf::Font &font, int &cols, int &rows, int &mines, string &name){
    int width = cols * 32;
    int height = (rows * 32) + 100;
    int minesLeft = mines;
    enum gameState { play, pause, debug, win, lose };
    bool leaderboardUpdated = false;
    gameState current = play;
    gameState previous = play;
    Board board(cols,  rows, mines);
    sf::RenderWindow window(sf::VideoMode(width, height), "Minesweeper", sf::Style::Close);
    window.clear(sf::Color::White);


    //Load Textures;
    sf::Texture hiddenTileTexture;
    hiddenTileTexture.loadFromFile("files/images/tile_hidden.png");

    sf::Texture shownTileTexture;
    shownTileTexture.loadFromFile("files/images/tile_revealed.png");

    sf::Texture flagTexture;
    flagTexture.loadFromFile("files/images/flag.png");

    sf::Texture mineTexture;
    mineTexture.loadFromFile("files/images/mine.png");

    vector<sf::Texture> mineCountTextures;
    sf::Texture mineOneTexture;
    mineOneTexture.loadFromFile("files/images/number_1.png");
    mineCountTextures.push_back(mineOneTexture);

    sf::Texture mineTwoTexture;
    mineTwoTexture.loadFromFile("files/images/number_2.png");
    mineCountTextures.push_back(mineTwoTexture);

    sf::Texture mineThreeTexture;
    mineThreeTexture.loadFromFile("files/images/number_3.png");
    mineCountTextures.push_back(mineThreeTexture);

    sf::Texture mineFourTexture;
    mineFourTexture.loadFromFile("files/images/number_4.png");
    mineCountTextures.push_back(mineFourTexture);

    sf::Texture mineFiveTexture;
    mineFiveTexture.loadFromFile("files/images/number_5.png");
    mineCountTextures.push_back(mineFiveTexture);

    sf::Texture mineSixTexture;
    mineSixTexture.loadFromFile("files/images/number_6.png");
    mineCountTextures.push_back(mineSixTexture);

    sf::Texture mineSevenTexture;
    mineSevenTexture.loadFromFile("files/images/number_7.png");
    mineCountTextures.push_back(mineSevenTexture);

    sf::Texture mineEightTexture;
    mineEightTexture.loadFromFile("files/images/number_8.png");
    mineCountTextures.push_back(mineEightTexture);

    sf::Texture debugTexture;
    debugTexture.loadFromFile("files/images/debug.png");

    sf::Texture happyFaceTexture;
    happyFaceTexture.loadFromFile("files/images/face_happy.png");

    sf::Texture loseFaceTexture;
    loseFaceTexture.loadFromFile("files/images/face_lose.png");

    sf::Texture winFaceTexture;
    winFaceTexture.loadFromFile("files/images/face_win.png");

    sf::Texture pauseTexture;
    pauseTexture.loadFromFile("files/images/pause.png");

    sf::Texture playTexture;
    playTexture.loadFromFile("files/images/play.png");

    sf::Texture leaderboardTexture;
    leaderboardTexture.loadFromFile("files/images/leaderboard.png");

    vector<sf::Sprite> digitList;

    sf::Texture digitsTexture;
    digitsTexture.loadFromFile("files/images/digits.png");
    sf::Sprite digits;
    digits.setTexture(digitsTexture);

    sf::IntRect rect0(0, 0, 21, 32);
    digits.setTextureRect(rect0);
    sf::Sprite digit0 = digits;
    digitList.push_back(digit0);

    sf::IntRect rect1(21, 0, 21, 32);
    digits.setTextureRect(rect1);
    sf::Sprite digit1 = digits;
    digitList.push_back(digit1);

    sf::IntRect rect2(21 * 2, 0, 21, 32);
    digits.setTextureRect(rect2);
    sf::Sprite digit2 = digits;
    digitList.push_back(digit2);

    sf::IntRect rect3(21 * 3, 0, 21, 32);
    digits.setTextureRect(rect3);
    sf::Sprite digit3 = digits;
    digitList.push_back(digit3);

    sf::IntRect rect4(21 * 4, 0, 21, 32);
    digits.setTextureRect(rect4);
    sf::Sprite digit4 = digits;
    digitList.push_back(digit4);

    sf::IntRect rect5(21 * 5, 0, 21, 32);
    digits.setTextureRect(rect5);
    sf::Sprite digit5 = digits;
    digitList.push_back(digit5);

    sf::IntRect rect6(21 * 6, 0, 21, 32);
    digits.setTextureRect(rect6);
    sf::Sprite digit6 = digits;
    digitList.push_back(digit6);

    sf::IntRect rect7(21 * 7, 0, 21, 32);
    digits.setTextureRect(rect7);
    sf::Sprite digit7 = digits;
    digitList.push_back(digit7);

    sf::IntRect rect8(21 * 8, 0, 21, 32);
    digits.setTextureRect(rect8);
    sf::Sprite digit8 = digits;
    digitList.push_back(digit8);

    sf::IntRect rect9(21 * 9, 0, 21, 32);
    digits.setTextureRect(rect9);
    sf::Sprite digit9 = digits;
    digitList.push_back(digit9);

    sf::IntRect negativeSymbol(21 * 10, 0, 21, 32);
    digits.setTextureRect(negativeSymbol);
    sf::Sprite negative = digits;
    negative.setPosition(12, 32 * (rows + 0.5f) +16);
    digitList.push_back(negative);



    //Timer
    auto startTime = chrono::high_resolution_clock::now();
    //Sets to 0 Automatically
    auto pauseTime = chrono::high_resolution_clock::now();
    auto totalPauseTime = chrono::duration_cast<chrono::seconds>(chrono::high_resolution_clock::now() - pauseTime).count(); // int



    while(window.isOpen()){
        window.clear(sf::Color::White);
        sf::Event event;
        while(window.pollEvent(event)) {
            window.clear(sf::Color::White);
            if (minesLeft == 0 && (current == play || current == debug)) {
                bool check = true;
                for (auto iter: board.mineLocations) {
                    if (!iter->isFlagged()) {
                        check = false;
                    }
                }
                if (check){
                    current = win;

                }
            }
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i position = sf::Mouse::getPosition(window);
                if ((position.y < (rows * 32)) && (position.x < (cols * 32))) {
                    Tile *pressedTile = board.boardPieces[(position.y / 32) + 1][(position.x / 32) + 1];
                    if (event.mouseButton.button == sf::Mouse::Left && ((current == play) || (current == debug)) &&
                        (!pressedTile->isVisible()) && (!pressedTile->isFlagged())) {
                        if (pressedTile->isMine()) {
                            current = lose;

                        } else {
                            board.checkTile(pressedTile);
                            if (pressedTile->getAdjMineCount() > 0) {
                                pressedTile->makeVisible();
                            }
                        }
                    } else if (event.mouseButton.button == sf::Mouse::Right) {
                        pressedTile->toggleFlag();
                        if (pressedTile->isFlagged()) {
                            minesLeft -= 1;
                        } else if (!pressedTile->isFlagged()) {
                            minesLeft += 1;
                        }
                    }
                }
                else if((((cols * 32) - 304) <= position.x) && (position.x <= (((cols * 32) - 304) + 64)) && ((((rows + 0.5f) * 32) <= position.y) && (position.y <= (((rows + 0.5f) * 32) +  64))) && (current == play || current == debug)){
                    if (current != debug) {
                        current = debug;
                    }
                    else {
                        current = play;
                    }
                }
                else if(((((cols / 2.0) * 32) - 32) <= position.x) && (position.x <= ((((cols / 2.0) * 32) - 32) + 64)) && ((((rows + 0.5f) * 32) <= position.y) && (position.y <= (((rows + 0.5f) * 32) +  64)))){
                    window.close();
                    gameWindow(font, cols, rows, mines, name);
                }
                else if((((cols * 32) - 240) <= position.x) && (position.x <= (((cols * 32) - 240) + 64)) && ((((rows + 0.5f) * 32) <= position.y) && (position.y <= (((rows + 0.5f) * 32) +  64)))){
                    if (current != pause) {
                        current = pause;
                        pauseTime = chrono::high_resolution_clock::now();
                    }
                    else if (current == pause){
                        current = play;
                        auto playTime = chrono::high_resolution_clock::now();
                        totalPauseTime += chrono::duration_cast<chrono::seconds>(playTime - pauseTime).count(); // add paused time
                    }
                }
                else if((((cols * 32) - 176) <= position.x) && (position.x <= (((cols * 32) - 176) + 64)) && ((((rows + 0.5f) * 32) <= position.y) && (position.y <= (((rows + 0.5f) * 32) +  64)))){
                    pauseTime = chrono::high_resolution_clock::now();
                    leaderboardWindow(font, cols, rows);
                    auto playTime = chrono::high_resolution_clock::now();
                    totalPauseTime += chrono::duration_cast<chrono::seconds>(playTime - pauseTime).count(); // add paused time
                }
            }
        }
        //Draw the Window
        //draw timer
        auto gameDuration = chrono::duration_cast<chrono::seconds>(chrono::high_resolution_clock::now() - startTime);
        int totalDuration = gameDuration.count();

        int minutes;
        int seconds;

        if (current == play || current == debug) {
            totalDuration = totalDuration - totalPauseTime;
            minutes = totalDuration / 60;
            seconds = totalDuration % 60;
        }

        int minutesTens = minutes / 10 % 10;
        int minutesZeros = minutes % 10;
        int secondsTens = seconds / 10 % 10;
        int secondsZeros = seconds % 10;

        if(current == win && !leaderboardUpdated){
            ifstream fileIn("files/leaderboard.txt");
            ofstream fileOut("files/tempFile.txt");
            bool inputAdded = false;
           for(int i = 1;  i <= 5; i++){
               string line;
               getline(fileIn, line);
               int comma = line.find(',');
               int colon = line.find(':');
               int min = stoi(line.substr(0, colon));
               int sec = stoi(line.substr(colon + 1, comma));
               string lbName = line.substr(comma + 2, line.size() - 2);

               if((minutes < min || (min == minutes && seconds < sec)) && !inputAdded){
                   fileOut << setw(2) << setfill('0') << minutes << ":" << setw(2) << setfill('0') << seconds << ", " << name << "*" << endl;
                   fileOut << line << endl;
                   i += 1;
                   inputAdded = true;
               }
               else{
                   fileOut << line << endl;
               }

           }
            rename("files/tempFile.txt", "files/leaderboard.txt");
           leaderboardUpdated = true;
        }

        digitList[minutesTens].setPosition((cols * 32) - 97, 32 * (rows + 0.5f) + 16);
        window.draw(digitList[minutesTens]);
        digitList[minutesZeros].setPosition((cols * 32) - 97 + 21, 32 * (rows + 0.5f) + 16);
        window.draw(digitList[minutesZeros]);
        digitList[secondsTens].setPosition((cols * 32) - 54, 32 * (rows + 0.5f) + 16);
        window.draw(digitList[secondsTens]);
        digitList[secondsZeros].setPosition((cols * 32) - 54 + 21, 32 * (rows + 0.5f) + 16);
        window.draw(digitList[secondsZeros]);

        int temp = minesLeft;
        if (temp < 0) {
            negative.setPosition(12, 32 * (rows + 0.5) + 16);
            window.draw(negative);
            temp *= -1;
        }
        if(temp == 0){
            digitList[0].setPosition(33,32 * (rows + 0.5) + 16);
            window.draw(digitList[0]);
        }

        int ticker = to_string(temp).size() - 1;
        while(temp > 0) {
            int singleDigit = temp % 10;
            digitList[singleDigit].setPosition(33 + (ticker * 21), 32 * (rows + 0.5f) + 16);
            window.draw(digitList[singleDigit]);
            temp /= 10;
            ticker--;
        }



        sf::Sprite button;
        if(current == lose){
            button.setTexture(loseFaceTexture);
            button.setPosition(((cols / 2.0) * 32) - 32, (rows + 0.5f) * 32);
            window.draw(button);
        }
        else if (current == win){
            button.setTexture(winFaceTexture);
            button.setPosition(((cols / 2.0) * 32) - 32, (rows + 0.5f) * 32);
            window.draw(button);
        }
        else if (current == play || current == debug){
            button.setTexture(happyFaceTexture);
            button.setPosition(((cols / 2.0) * 32) - 32, (rows + 0.5f) * 32);
            window.draw(button);
            button.setTexture(pauseTexture);
            button.setPosition((cols * 32) - 240, (rows + 0.5f) * 32);
            window.draw(button);
        }
        else if (current == pause){
            button.setTexture(happyFaceTexture);
            button.setPosition(((cols / 2.0) * 32) - 32, (rows + 0.5f) * 32);
            window.draw(button);
            button.setTexture(playTexture);
            button.setPosition((cols * 32) - 240, (rows + 0.5f) * 32);
            window.draw(button);
        }

        button.setTexture(debugTexture);
        button.setPosition((cols * 32) - 304, (rows + 0.5f) * 32);
        window.draw(button);
        button.setTexture(leaderboardTexture);
        button.setPosition((cols * 32) - 176, (rows + 0.5f) * 32);
        window.draw(button);


        sf::Sprite currentSprite;
        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                currentSprite.setPosition(sf::Vector2f(j * 32, i * 32));
                Tile* currentTile = board.boardPieces[i + 1][j + 1];
                if(current == pause){
                    currentSprite.setTexture(shownTileTexture);
                    window.draw(currentSprite);
                }
                else{
                    if(current == debug){
                        if(currentTile->isFlagged()){
                            currentSprite.setTexture(hiddenTileTexture);
                            window.draw(currentSprite);
                            currentSprite.setTexture(flagTexture);
                            window.draw(currentSprite);
                        }
                        else{

                            if(currentTile->isMine()) {
                                currentSprite.setTexture(hiddenTileTexture);
                                window.draw(currentSprite);
                                currentSprite.setTexture(mineTexture);
                                window.draw(currentSprite);
                            }
                            else if(currentTile->getAdjMineCount() > 0 && currentTile->isVisible()){
                                currentSprite.setTexture(shownTileTexture);
                                window.draw(currentSprite);
                                currentSprite.setTexture(mineCountTextures[currentTile->getAdjMineCount() - 1]);
                                window.draw(currentSprite);
                            }
                            else if (currentTile->isVisible()){
                                currentSprite.setTexture(shownTileTexture);
                                window.draw(currentSprite);
                            }
                            else{
                                currentSprite.setTexture(hiddenTileTexture);
                                window.draw(currentSprite);
                            }
                        }
                    }
                    else{
                        if(currentTile->isVisible()){
                            currentSprite.setTexture(shownTileTexture);
                            window.draw(currentSprite);
                            if(currentTile->getAdjMineCount() > 0){
                                currentSprite.setTexture(mineCountTextures[currentTile->getAdjMineCount() - 1]);
                                window.draw(currentSprite);
                            }
                        }
                        else if(!currentTile->isVisible()){
                            currentSprite.setTexture(hiddenTileTexture);
                            window.draw(currentSprite);
                            if((current == lose) && currentTile->isMine()){
                                currentSprite.setTexture(mineTexture);
                                window.draw(currentSprite);
                                continue;
                            }
                            if(currentTile->isFlagged()){
                                currentSprite.setTexture(flagTexture);
                                window.draw(currentSprite);
                            }
                        }

                    }
                }
            }
        }
    window.display();
    }
}

void leaderboardWindow(sf::Font &font, int &cols, int &rows){
    int width = cols * 16;
    int height = (rows * 16) + 50;
    ifstream fileIn("files/leaderboard.txt");
    sf::RenderWindow leaderboard(sf::VideoMode(width, height), "Leaderboard", sf::Style::Close);
    leaderboard.clear(sf::Color::Blue);

    sf::Text title, body;
    title.setFont(font);
    title.setString("LEADERBOARD");
    title.setFillColor(sf::Color::White);
    title.setStyle(sf::Text::Bold | sf::Text::Underlined);
    title.setCharacterSize(20);
    setText(title, (width / 2), (height / 2) - 120);
    string fullText;
    for(int i = 1; i <= 5; i++){
        string line;
        getline(fileIn, line);
        int delimiter = line.find(',');
        fullText += to_string(i) + ".";
        fullText += "\t";
        fullText += line.substr(0, delimiter);
        fullText += "\t";
        fullText += line.substr(delimiter + 1, line.size() - delimiter);
        fullText += "\n\n";
    }
    body.setFont(font);
    body.setString(fullText);
    body.setStyle(sf::Text::Bold);
    body.setFillColor(sf::Color::White);
    body.setCharacterSize(18);
    setText(body, (width / 2), (height / 2) + 20);

    leaderboard.draw(title);
    leaderboard.draw(body);
    leaderboard.display();


    while (leaderboard.isOpen()) {
        sf::Event event;
        while (leaderboard.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                leaderboard.close();
            }
        }
        leaderboard.clear(sf::Color::Blue);
        leaderboard.draw(title);
        leaderboard.draw(body);
        leaderboard.display();
    }

}

int main() {
    ifstream config("files/config.cfg");
    int cols, rows, mines;
    config >> cols >> rows >> mines;
    config.close();
    sf::Font font;
    font.loadFromFile("files/font.ttf");
    startWindow(font, cols, rows, mines);
}