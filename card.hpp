#include <algorithm>
#include <iostream>
#include <random>
#include <string>
#include <vector>

typedef class hand_card hand_card;
// static　で持つより、externしてからグローバルに持つほうがわかりやすいし　扱いやすい？
extern std::vector<hand_card> hand;
extern int all_plyr_num;

void card_error();

// my_plyr_num　を参照することで　カード整理しやすくする
// 宣言をしたときに、player人数とjoker数（等）を指定して初期化-> game内一定で、staticで規定することに変更
// vector<hand_card>　の形で使用することを想定している

class hand_card{
public:
    std::vector<int> card;
    int my_plyr_num;

// gameを通して全プレイヤー＆山札が共通して持つ変数,mainで指定
    // deckのみが持つ。cardがそれぞれどこにあるか、0ならdeck,他はplyrのnumber -1:処理保留（移動中） 
    static std::vector<int> card_status;
    static int JOKER;

    // int sz();
    int top(int def=0);
    int bottom(int def=0);

    // [deck -> hand] : hand.draw(deck.draw());
    int move(int val,hand_card &from,hand_card &to); // A(/B) .move(A.top(),A,B) : A->B
    void deal_card();

    // int drawfrom(hand_card A);
// main　で定義したvector 内をループで回す形で使用。my_plyr_numを見ることで　山札用/手札用の処理の違いを実現
    void init_card();

// 手札名を指定して初期化、山札でのみ異なった挙動
    // void init(int plyr_num);
    // すべてを初期化
    // void init_all();

    void shuffle();
    void O_card(int def=1);

// static 変数にアクセスするためstatic

    static int get_status(int val);
    static int set_status(int val,int to);
    static bool cng_status(int val,int from,int to=-1);
    static void init_status();
    
    static int decksize();

// 独立して使用する可能性があるためstatic
    static std::string cng_spell(int val);
    static std::string stnm(int i);

private:
};
