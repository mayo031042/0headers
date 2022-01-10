// timer として使用
// コンストラクタで１フレームの時間を指定する
// 引数の単位は　秒

// 想定される使用法
// １ remain() により現フレームの終端までの残余時間を取得 (1/1000 秒単位)
// ２　is_over() により経過時間を取得し_time クラス外で条件分岐
// ３　adjustment() により１フレームを刻むまでNOP
// 2,3 はどちらも１フレームが経過したことを知らせるタイミングでreset() をする

#ifndef mychrn
#define mychrn

#include <chrono>

// loop_unit -> 目安は0.050ms(5e-2)以上？
// 保持するのは直前に刻んだ時刻と　ループ回数のみ
class _time
{
private:
    std::chrono::system_clock::time_point latest; // 直前に打刻した時間

    double loop_unit;     // フレームループ単位 1/1000秒単位
    long long loop_times; // 打刻回数 始まった瞬間は０フレーム目

    // latest に現在時刻をセット つまりタイマーリセット
    void stamp()
    {
        latest = std::chrono::system_clock::now();
    }

    // 直前に打刻してからの経過時間
    double progress()
    {
        return static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - latest).count() / 1000.0);
    }

    // タイマーリセットして打刻回数を１つ増やす
    void reset()
    {
        stamp();
        ++loop_times;
    }

public:
    _time(double loop_sec = 1) : loop_unit(loop_sec * 1000), loop_times(-1)
    {
        reset();
    }

    // 現フレームの末端までの残余時間 (1/1000 秒単位)
    double remain()
    {
        return loop_unit - progress();
    }

    // ループ単位以上の時間が過ぎていたらtrue
    bool is_over()
    {
        if (remain() < 0)
        {
            reset();
            return true;
        }

        return false;
    }

    // 目標のloop開始時間になるまでNOP する
    void adjustment()
    {
        while (!is_over())
        {
            continue;
        }

        reset();
    }
};

#endif