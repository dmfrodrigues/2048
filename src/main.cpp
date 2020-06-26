/**
 * (C) 2018-2020 Diogo Rodrigues
 */

#include <SFML/Graphics.hpp>
    #include "RoundedRectangleShape.hpp"
#include <SFML/Audio.hpp>

#include <vector>

#include "myConversions.h"

//#include <windows.h>

#include <iostream>

/**TYPEDEFS============================================*/
typedef unsigned long int sizeT;
typedef   signed long int sizeT_u;
/**INPUT===============================================*/
namespace Input{
    enum {
        INVALID   = -1,
        Dir_Right = 0,
        Dir_Up    = 1,
        Dir_Left  = 2,
        Dir_Down  = 3,
        Evt_Close = 4
    };
    int GetInput(sf::RenderWindow& win){

        sf::Keyboard keyboard;
        sf::Keyboard::Key k = sf::Keyboard::Key::Unknown;

        sf::Event event;

        while(true){
            sf::sleep(sf::milliseconds(10));
            if(win.hasFocus()){
                ///EVENTS
                while(win.pollEvent(event)){
                    switch(event.type){
                        case sf::Event::Closed: return Evt_Close;
                        default: break;
                    }
                }
                ///KEYBOARD
                k = sf::Keyboard::Key::Unknown;
                if(keyboard.isKeyPressed(sf::Keyboard::Key::Right)) k = sf::Keyboard::Key::Right;
                if(keyboard.isKeyPressed(sf::Keyboard::Key::Up   )) k = sf::Keyboard::Key::Up   ;
                if(keyboard.isKeyPressed(sf::Keyboard::Key::Left )) k = sf::Keyboard::Key::Left ;
                if(keyboard.isKeyPressed(sf::Keyboard::Key::Down )) k = sf::Keyboard::Key::Down ;
                if(k != sf::Keyboard::Key::Unknown){
                    while(keyboard.isKeyPressed(k)){
                        sf::sleep(sf::milliseconds(10));
                    }
                    switch(k){
                        case sf::Keyboard::Key::Right: return Dir_Right;
                        case sf::Keyboard::Key::Up   : return Dir_Up   ;
                        case sf::Keyboard::Key::Left : return Dir_Left ;
                        case sf::Keyboard::Key::Down : return Dir_Down ;
                        default: break;
                    }
                }
            }
        }
    }
}
/**TILE================================================*/
class Tile{
public:
    int Number;
    bool Joined;
    Tile(const int& Number_, const bool& Joined_ = false):
        Number(Number_),Joined(Joined_){}
    bool operator==(const Tile& obj)const{
        return((this->Number == obj.Number) &&
               (this->Joined == obj.Joined));
    }
};
/**BOARD===============================================*/
class Board: public std::vector< std::vector<Tile> >{
private:
    sizeT sz1, sz2;
public:
    Board(sizeT sz1_, sizeT sz2_, const Tile& Init):
        std::vector< std::vector<Tile> >(sz1_, std::vector<Tile>(sz2_, Init)),
        sz1(sz1_), sz2(sz2_)
        {}
    sizeT CountEmpty() const{
        sizeT ret = 0;
        for(sizeT x = 0; x < sz1; ++x){
            for(sizeT y = 0; y < sz2; ++y){
                if((*this)[x][y].Number == 0)
                    ++ret;
            }
        }
        return ret;
    }
    bool IsGameOver() const{
        if(CountEmpty() != 0){
            return false;
        }else{
            const Board& obj = *this;
            ///Direita
            for(sizeT x = 0; x < sz1-1; ++x)
                for(sizeT y = 0; y < sz2; ++y)
                    if(obj[x][y].Number == obj[x+1][y].Number) return false;
            ///Baixo
            for(sizeT x = 0; x < sz1; ++x)
                for(sizeT y = 0; y < sz2-1; ++y)
                    if(obj[x][y].Number == obj[x][y+1].Number) return false;
            return true;
        }
    }
    void ClearJoined(){
        for(size_type x = 0; x < sz1; ++x)
            for(size_type y = 0; y < sz2; ++y)
                (*this)[x][y].Joined = false;
    }
    int GetMax() const{
        int ret = 0;
        for(sizeT x = 0; x < sz1; ++x){
            for(sizeT y = 0; y < sz2; ++y){
                ret = std::max(ret, (*this)[x][y].Number);
            }
        }
        return ret;
    }
};
/**SOUND,COLOR,FONT,CHARSIZE===========================*/
sf::SoundBuffer newScore_sound_buffer;
sf::Sound newScore_sound;
inline sf::Color GetShapeColor(const int& N){
    switch(N){
        case 2  : return sf::Color(190,180,160,200); ///X
        case 4  : return sf::Color(195,170,170,200);
        case 8  : return sf::Color(200,100,100,200); ///X
        case 16 : return sf::Color(220, 80, 80,200);
        case 32 : return sf::Color(240, 60, 60,200);
        case 64 : return sf::Color(250, 30, 30,200); ///X
        case 128: return sf::Color(255,  5,  5,200);
        case 256: return sf::Color(250, 70, 70,200);
        case 512: return sf::Color(250,150,150,200); ///X
        default : return sf::Color(250,250,250,200); ///X
    }
}
inline sf::Color GetTextColor(const sf::Color& c){
    int n = (c.r + c.g + c.b)/3;
    if(n > 185) return sf::Color( 20, 20, 20,220);
    else        return sf::Color(230,230,230,200);
}
sf::Font font;
const unsigned int charSz_large = 55;
const unsigned int charSz_small = 35;
inline int GetCharSize(const sf::String& str){
    return (str.getSize() <= 2? charSz_large : charSz_small);
}
/**TILEDRAW============================================*/
const float TileSize = 100;
const int radius = 30;
class TileDraw: public sf::RoundedRectangleShape{
public:
    int N;
    sf::Text txt;
    TileDraw(const int& N_, const sf::Vector2f& pos):
        sf::RoundedRectangleShape(sf::Vector2f(TileSize, TileSize), radius/2, 20),
        N(N_),
        txt(mysprintf(N_, "%d"), font)
    {
        setPosition_(pos);
    }
    void setPosition_(const sf::Vector2f& pos){
        setPosition(pos);
        UpdateText();
    }
    void UpdateText(){
        txt.setCharacterSize(GetCharSize(txt.getString()));
        txt.setFillColor(sf::Color(0,0,0,255));
        txt.setPosition(getPosition() + sf::Vector2f(-0.02*txt.getCharacterSize(),-0.35*txt.getCharacterSize()) +
                        sf::Vector2f(TileSize/2-txt.getLocalBounds().width/2,TileSize/2-txt.getLocalBounds().height/2));
        setFillColor(GetShapeColor(this->N));
        txt.setFillColor(GetTextColor(getFillColor()));
    }
    void draw_(sf::RenderWindow& win)const{
        win.draw(*this);
        win.draw(this->txt);
    }
};
/**BOARDDRAW===========================================*/
const sizeT sz_ = 4;
const float UpMargin = 40;
const float TileMargin = 7;
const float TileBoardMargin = 14;
const float BoardMargin = 35;
const sf::Vector2f WinSz(TileSize*sz_ + TileMargin*(sz_-1) + 2*TileBoardMargin + 2*BoardMargin,
                         TileSize*sz_ + TileMargin*(sz_-1) + 2*TileBoardMargin + 2*BoardMargin + UpMargin);
const sf::Vector2f Pos0(BoardMargin + TileBoardMargin, UpMargin + BoardMargin + TileBoardMargin);
const float TileDif = TileSize + TileMargin;
class BoardDraw: public std::vector< std::vector<TileDraw*> >{
public:
    BoardDraw(const sizeT& sz1_, const sizeT& sz2_, TileDraw* ptr):
        std::vector< std::vector<TileDraw*> >(sz1_, std::vector<TileDraw*>(sz2_, ptr)){}
    void Nullify(){
        for(std::vector<TileDraw*>& line : *this){
            for(TileDraw*& ptr : line){
                delete(ptr);
                ptr = NULL;
            }
        }
    }
    void CreateFromBoard(const Board& board){
        Nullify();
        for(sizeT x = 0; x < sz_; ++x){
            for(sizeT y = 0; y < sz_; ++y){
                if(board[x][y].Number != 0){
                    (*this)[x][y] = new TileDraw(board[x][y].Number, Pos0 + sf::Vector2f(x*TileDif, y*TileDif));
                }
            }
        }
    }
};
/**WINDOW2048==========================================*/
const float stepTravel = TileDif/16;
const std::pair<int,int> alphaLim(100, 250); const int alphaStep = 10;
class Window2048: public sf::RenderWindow {
private:
    ///AESTHETICS
    sf::VertexArray Background = sf::VertexArray(sf::Quads, 4);
    sf::RoundedRectangleShape BoardRect = sf::RoundedRectangleShape(sf::Vector2f(WinSz.x-2*BoardMargin, WinSz.y-2*BoardMargin-UpMargin), radius, 20);
    std::vector<sf::RoundedRectangleShape> TilesBack =
        std::vector<sf::RoundedRectangleShape>(sz_*sz_, sf::RoundedRectangleShape(sf::Vector2f(TileSize,TileSize), radius/2, 10));
    sf::Text txtScore = sf::Text("Score: 0", font, charSz_small);
    void CreateShapes(){
        ///BACKGROUND
        Background[0].position = sf::Vector2f(      0,      0); Background[0].color = sf::Color(150,150,150);
        Background[1].position = sf::Vector2f(      0,WinSz.y); Background[1].color = sf::Color(180,180,180);
        Background[2].position = sf::Vector2f(WinSz.x,WinSz.y); Background[2].color = sf::Color(210,210,210);
        Background[3].position = sf::Vector2f(WinSz.x,      0); Background[3].color = sf::Color(180,180,180);
        ///BOARD
        BoardRect.setPosition(sf::Vector2f(BoardMargin, BoardMargin+UpMargin));
        BoardRect.setFillColor(sf::Color(255,128,0, 100));
        ///TILES
        sizeT i;
        for(sizeT x = 0; x < sz_; ++x){
            for(sizeT y = 0; y < sz_; ++y){
                i = x*sz_ + y;
                TilesBack[i].setPosition(sf::Vector2f(BoardMargin + TileBoardMargin + x*(TileSize + TileMargin),
                                                      BoardMargin + TileBoardMargin + y*(TileSize + TileMargin) + UpMargin));
                TilesBack[i].setFillColor(sf::Color(200,190,180,200));
            }
        }
        ///SCORE
        txtScore.setPosition(sf::Vector2f( 0,-0.35*txtScore.getCharacterSize())+
                             sf::Vector2f( 0,(UpMargin+BoardMargin)/2 - txtScore.getLocalBounds().height/2)+
                             sf::Vector2f(0.08*WinSz.x, 0.03*WinSz.y));
        txtScore.setFillColor(sf::Color(250,250,250,230));
    }
    inline void DrawAll(){
        draw(Background);
        draw(txtScore);
        draw(BoardRect);
        for(sf::RoundedRectangleShape& obj : TilesBack)
            draw(obj);
    }
    ///BOARDDRAW
    BoardDraw boarddraw = BoardDraw(sz_, sz_, NULL);
    inline void Draw_boarddraw(){
        for(sizeT x = 0; x < sz_; ++x){
            for(sizeT y = 0; y < sz_; ++y){
                if(boarddraw[x][y] != NULL){
                    boarddraw[x][y]->draw_(*this);
                }
            }
        }
    }
    ///SCORE
    int Score = 0;
    inline void UpdateScore(const int& n){
        txtScore.setString(mysprintf(Score += n, "Score: %d"));
    }
    ///BOARD
    Board board = Board(sz_, sz_, Tile(0));
    ///FUNCTIONS
    void Introduce(){
        const sizeT& countEmpty = board.CountEmpty();
        if(countEmpty != 0){
            sizeT index = rand()%countEmpty;
            sizeT i = 0;
            sizeT x, y;
            while(index != 0){
                y = i/sz_;
                x = i%sz_;
                if(board[x][y].Number == 0)
                    --index;
                ++i;
            }
            do{
                y = i/sz_;
                x = i%sz_;
                ++i;
            }while(board[x][y].Number != 0);
            board[x][y].Number = 2*(rand()%2) + 2;
            boarddraw.CreateFromBoard(board);
        }
        clear();
        DrawAll();
        Draw_boarddraw();
        display();
    }
    void Move(const int& dir){
        Board prevBoard = board;
        std::vector< std::vector<bool> > WasMoved(sz_, std::vector<bool>(sz_, false));

        std::pair<int,int> xLim; int xStep; int xNext;
        std::pair<int,int> yLim; int yStep; int yNext;
        switch(dir){
        case Input::Dir_Right:
            xLim = std::pair<int,int>(sz_-2,  -1); xStep = -1; xNext = +1;
            yLim = std::pair<int,int>(    0, sz_); yStep = +1; yNext = 00;
    break;
        case Input::Dir_Up:
            xLim = std::pair<int,int>(    0, sz_); xStep = +1; xNext = 00;
            yLim = std::pair<int,int>(    1, sz_); yStep = +1; yNext = -1;
            break;
        case Input::Dir_Left:
            xLim = std::pair<int,int>(    1, sz_); xStep = +1; xNext = -1;
            yLim = std::pair<int,int>(    0, sz_); yStep = +1; yNext = 00;
            break;
        case Input::Dir_Down:
            xLim = std::pair<int,int>(    0, sz_); xStep = +1; xNext = 00;
            yLim = std::pair<int,int>(sz_-2,  -1); yStep = -1; yNext = +1;
            break;
        }

        for(int x = xLim.first; x != xLim.second; x+=xStep){
            for(int y = yLim.first; y != yLim.second; y+=yStep){
                if(board[x][y].Number != 0){
                    Tile& prev = board[x      ][y      ];
                    Tile& next = board[x+xNext][y+yNext];
                    if(next.Number == 0){ next = prev; prev.Number = 0; prev.Joined = false; WasMoved[x][y] = true; }
                    if( prev.Number ==  next.Number && !prev.Joined && !next.Joined){
                        UpdateScore(2*prev.Number);
                        next.Number *= 2; next.Joined = true;
                        prev.Number = 0; prev.Joined = false;
                        WasMoved[x][y] = true;
                    }
                }
            }
        }

        if(prevBoard != board){
            sf::Vector2f stepVtr;
            switch(dir){
                case Input::Dir_Right: stepVtr = sf::Vector2f(+stepTravel, 0); break;
                case Input::Dir_Up   : stepVtr = sf::Vector2f(0, -stepTravel); break;
                case Input::Dir_Left : stepVtr = sf::Vector2f(-stepTravel, 0); break;
                case Input::Dir_Down : stepVtr = sf::Vector2f(0, +stepTravel); break;
            }

            float acc = 0;
            while(acc < TileDif){
                clear();
                DrawAll();
                for(sizeT x = 0; x < sz_; ++x){
                    for(sizeT y = 0; y < sz_; ++y){
                        if(boarddraw[x][y] != NULL && WasMoved[x][y])
                            boarddraw[x][y]->setPosition_(boarddraw[x][y]->getPosition()+stepVtr);
                    }
                }
                Draw_boarddraw();

                display();
                acc += stepTravel;
            }
            boarddraw.CreateFromBoard(board);

            clear();
            DrawAll();
            Draw_boarddraw();
            display();
            this->Move(dir);
        }
    }
public:
    Window2048(): sf::RenderWindow(sf::VideoMode(WinSz.x, WinSz.y), "2048!", sf::Style::Titlebar | sf::Style::Close, sf::ContextSettings(0,0,8)) {

        sf::Image icon2048;
        icon2048.loadFromFile("resources/icon2048_transp.png");
        setIcon(icon2048.getSize().x, icon2048.getSize().y, icon2048.getPixelsPtr());

        setFramerateLimit(300);
        CreateShapes();
        DrawAll();
        display();
    }
    void Start(){

        Introduce();
        Introduce();

        Board prevBoard(board);

        while(!board.IsGameOver()){
            prevBoard = board;
            int Command = Input::GetInput(*this);
            if(Command == Input::Evt_Close){
                close();
                return;
            }
            Move(Command);
            board.ClearJoined();
            if(board != prevBoard)
                Introduce();

            if(board.GetMax() > prevBoard.GetMax()){
                newScore_sound.play();
                for(int i = alphaLim.second; i > alphaLim.first; i-=alphaStep){
                    Background[0].color.a = Background[1].color.a = Background[2].color.a = Background[3].color.a = i;
                    clear(sf::Color(250,0,0,255));
                    DrawAll();
                    Draw_boarddraw();
                    display();
                }
                for(int i = alphaLim.first; i < alphaLim.second; i+=alphaStep){
                    Background[0].color.a = Background[1].color.a = Background[2].color.a = Background[3].color.a = i;
                clear(sf::Color(250,0,0,255));

                    DrawAll();
                    Draw_boarddraw();
                    display();
                }
                Background[0].color.a = Background[1].color.a = Background[2].color.a = Background[3].color.a = 255;
                clear();
                DrawAll();
                Draw_boarddraw();
                display();
            }
        }
        sf::RectangleShape rectFinal(WinSz);
        sf::Text txtFinal("Game over!", font, 115/80*charSz_large);
        txtFinal.setPosition(sf::Vector2f(WinSz.x/2 - txtFinal.getLocalBounds().width/2, 0.25*WinSz.y));
        sf::Text txtScore(mysprintf(Score, "Score: %d"), font, 1.15*charSz_small);
        txtScore.setPosition(sf::Vector2f(WinSz.x/2 - txtScore.getLocalBounds().width/2, 0.40*WinSz.y));
        sf::Text txtAutor(L"Diogo Rodrigues, nº 5\n"
                           "12ºB - 2017/18\n"
                           "API B - Prof. Daniel Prata\n"
                           "Escola Secundária do Castêlo da Maia", font, charSz_small*35/50);
        txtAutor.setPosition(sf::Vector2f(20, WinSz.y - txtAutor.getLocalBounds().height - 30));
        txtAutor.setFillColor(sf::Color(250,250,250,230));
        std::pair<float,float> alphaLim(0.0,200.0); float alphaStep = 0.05;
        for(float i = 0.0; i < 1.0; i+=alphaStep){
            rectFinal.setFillColor(sf::Color(160,160,160,alphaLim.first + i*(alphaLim.second - alphaLim.first)));
            txtFinal.setFillColor(sf::Color(255,255,255,i*250));
            txtScore.setFillColor(sf::Color(230,230,230,i*250));
            clear();
            DrawAll();
            Draw_boarddraw();
            draw(rectFinal);
            draw(txtFinal);
            draw(txtScore);
            draw(txtAutor);
            display();
        }
        while(Input::GetInput(*this) != Input::Evt_Close){}
    }
};
/**MAIN================================================*/
int main(){
    srand(time(NULL));

    font.loadFromFile("resources/brittanic-bold-regular.ttf");

    newScore_sound_buffer.loadFromFile("resources/newScore_sound_v01.wav");
    newScore_sound.setBuffer(newScore_sound_buffer);

    sf::Music backmusic;
    backmusic.setLoop(true);
    backmusic.setVolume(25);
    backmusic.openFromFile("resources/TheoTown_Song.ogg");
    backmusic.play();

    Window2048 win;
    win.Start();

    backmusic.stop();

    return 0;
}
