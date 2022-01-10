#include "card.hpp"

std::vector<std::string> suit_vec_str={"Spe","Hrt","Dia","Clb"}; 


// extern std::vector<hand_card> hand;


// 未実装
// static string の定義の仕方と、それにアクセスするためのstatic menber funcの書き方が結局わからなかった
// 山札から昇順で任意の最初の一枚を引く、そのあと、山札はシャッフルされない
// 山札から上　任意枚とる、そのあと、それを山札の下に持っていく (cut　する)
// member関数　からコンストラクタを引き出すことはできない？
// カードの整形箇所を変更することで、カードの強弱　を表現

using namespace std;

// < class 内で宣言されていない関数 >

// 絵札(JQK)なら1を、JOKERなら0を、それ以外なら-1をかえす
int jdg_face(int val){
    if(4*13<=val) return 0;
    if(10 <= val%13) return 1;
    return -1;
}
// トランプの数字を返す　JOKER なら0
int jdg_val(int val){
    if(4*13 <= val) return 0;
    return val%13+1;
}

// < / class 内で宣言されていない関数 >


// error
void card_error(){
    printf("card_error !\n");
    exit(1);
}

// 
// 山札の状態を確認
// 

// 山札の一番 上 からdef　枚目を見る　カードがないなら−１が返る　defaultは一番　上
int hand_card::top(int def){
    int i=card.size()-def-1; // 反転
    if(i<0) card_error();
    if(card.size()<=i) return -1;
    return card[i];
}

// 山札の一番 下 からdef　枚目を見る　カードがないなら−１が返る default は一番　下
int hand_card::bottom(int def){
    if(def<0) card_error();
    if(card.size()<=def) return -1;
    return card[def];
}

int hand_card::decksize(){
    return 4*13+JOKER;
}

// 
//　山札の並び替え
//  
// もとのデッキ枚数を復元する　シャッフルまで行うので、必要に応じてソートせよ
// これ一つで外部からも内部からも山札all手札の初期化ができるようにしたい    

void hand_card::init_status(){
    card_status.resize(decksize(),0);
}

void hand_card::init_card(){
    // 山札なら
    if(my_plyr_num==0){
        card.resize(decksize());

        for(int i=0;i<decksize();i++){
            cng_status(i,-1,0); // 山札へstatus change
            card[i]=i;
        }
        shuffle();
        return ;
    }
    // 手札なら
    for(int val:card) cng_status(val,my_plyr_num,0);
    card.resize(0);
}

void hand_card::shuffle(){
    int n=card.size();
    if(n<=0)return ;

    random_device rnd;
    mt19937 mt(rnd());

    vector<int> shfd=card;
    for(int &val:card)val=-1;

    for(int i=0;i<n;i++){
        int loc=mt()%n;
        while( card[loc]!=-1 ) (loc+=1)%=n;
        card[loc]=shfd[i];
    }
}

int hand_card::get_status(int val){
    return card_status[val];
}

int hand_card::set_status(int val,int to){
    return card_status[val]=to;
}

// bool　として使用、from->to のカードの移動が適切に行われるかを判定しつつ、適切な場合は(true)実行まで行う
bool hand_card::cng_status(int val,int from,int to){
    // val がfromか [-1] にセットされていたかどうかを判定する
    if(get_status(val)!=from and get_status(val)!=-1) return false;
    set_status(val,to);
    return 1;
}


int hand_card::move(int val,hand_card &A,hand_card &B){
    // val がA にないときはerror
    if(get_status(val)!=A.my_plyr_num) card_error();
    if(cng_status(val,A.my_plyr_num,B.my_plyr_num)==false) card_error();

    auto ite=std::find(A.card.begin(),A.card.end(),val);
    A.card.erase(ite);

    B.card.push_back(val);
    return val;
}

void hand_card::deal_card(){
    int now_plyr=0,n=decksize();
    
    for(int i=0;i<n;i++){
        hand[0].move(hand[0].top(),hand[0],hand[now_plyr+1]);
        now_plyr++;
        if(now_plyr==all_plyr_num)now_plyr=0;
    }
}


// 
// output 用の整形
// 
string hand_card::stnm(int i){
    if(i<0 or 4<=i) card_error(); // 範囲外
    return suit_vec_str[i];
}

// val != -1 : 値がvalであるようなカードの整形後を出力する　改行なし 
string hand_card::cng_spell(int val){
    if(4*13<=val) return "JOKER";

    int num=val%13+1;
    string suit_str=stnm(val/13);
    string num_str=" ";

    if(num==10) num_str="10";
    else if(num==1) num_str+="A";
    else if(1 < num and num < 10) num_str+=to_string(num);
    else if(num==11) num_str+="J";
    else if(num==12) num_str+="Q";
    else if(num==13) num_str+="K";
    
    return suit_str+num_str; 
}

// def -> [1,0,-1] : [sort, そのまま, そのままをreverse] その後改行
void hand_card::O_card(int def){
    vector<int> tmp=card;
    if(def==1) sort(tmp.begin(),tmp.end());
    if(def==-1) reverse(tmp.begin(),tmp.end());

    for(int val:tmp) cout<<cng_spell(val)<<"    ";
    cout<<endl;
}

