// 入力を解釈するクラス
// 解釈は一定期間ごとに行われることを想定（＝１フレーム）
// その間の入力は入力された順番で並ぶ
// とりあえず数字とアルファベット小文字のみ対応

// プライベート変数Input に格納された入力は
// １　Input を文字列として返す
// ２　任意の位置の文字を返す　範囲外なら\n を返す
// の２通りの操作のみ受け付ける

// プライベート配列Chs はInput に存在する文字に対してそれぞれの出現回数を記録する

// パブリック関数parse について
// 実行されたタイミングで標準入力に溜まっている入力をすべて？解釈する
// 実行されるたびにInput は初期化される

// 想定される使用法
// １フレーム単位でループさせるような実装の場合、
// 描画等処理
// 次回描画に影響するような入力の処理
// というループを想定しており、これは描画等処理にどれほどの時間が割かれるか不明な場合に
// 入力処理の時間を流動的に設置するための実装である

// 1 parse() フレームの終端まで実行される　標準入力への入力を出力なしで取得する
// 2 input() 入力された文字列を返す
// 3 len() 入力された文字列の長さを返す
// 4 [char c] 入力文字列の中から　引数に一致する文字の出現回数を返す

#ifndef _STDI_
#define _STDI_

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include "mychrn.hpp"

using namespace std;

typedef class _stdi _stdi;
class _stdi
{
private:
    static const int kinds = 50;
    int Chs[kinds];

    string Input;
    int Len;

    _time time;

    // 初期化
    void init()
    {
        Input = "";
        for (int i = 0; i < kinds; i++)
        {
            Chs[i] = 0;
        }
        Len = 0;
    }

    // 入力を 対応するChs[]のインデックス に読み替える
    // 対応するインデックスが見つからない場合は-1 を返す
    int replace(char c)
    {
        // 0-9 までの数字ならそのままChs[] の0-9 にマッピングする
        if (isdigit(c))
        {
            return c - '0';
        }
        // そうでないならアルファベット小文字なので順にマッピングする
        else if (isalpha(c))
        {
            return c - 'a';
        }

        return -1;
    }

public:
    // 空文字列と長さ０で初期化

    _stdi(double sec = 1)
    {
        init();
        _time time(sec);
    };

    // 任意のタイミングで入力を解析する
    // 直前までの入力は破棄される
    // 入力はInput,Len,Chs[] に格納される
    void parse()
    {
        init();

        struct termios oldattr, newattr;
        tcgetattr(0, &oldattr);
        newattr = oldattr;

        // 入力をエコーバックしない、バッファリングしない
        newattr.c_lflag &= ~(ECHO | ICANON);
        newattr.c_cc[VTIME] = 0;
        newattr.c_cc[VMIN] = 1;
        tcsetattr(0, TCSANOW, &newattr);

        // 標準入力からの読み込むときブロックしないようにする
        fcntl(0, F_SETFL, O_NONBLOCK);

        // getchar() によるバッファ読み込み？
        {
            char c;
            while (!time.is_over())
            {
                if ((c = getchar()) != EOF)
                {
                    Input += c;
                }
            }
            Len = Input.size();

            for (int i = 0; i < Len; i++)
            {
                Chs[replace(Input[i])]++;
            }
        }

        // 設定を復帰
        tcsetattr(0, TCSANOW, &oldattr);
    }

    // 入力された文字列を返す
    const string input()
    {
        return Input;
    }

    // 入力された文字列の長さを返す
    const int len()
    {
        return Len;
    }

    // 入力された文字列の中から引数に一致する文字が入力された回数を返す
    // 対応していないキーの参照にはエラーを吐く
    const int operator[](char c)
    {
        int index = replace(c);
        if (index < 0 || kinds <= index)
        {
            fprintf(stderr, "対応していないキーが入力されました\n");
            exit(0);
        }
        else
        {
            return Chs[index];
        }
    }
};

#endif