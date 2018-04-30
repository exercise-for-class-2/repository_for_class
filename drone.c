#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define GNPLT "C:/PROGRA~1/gnuplot/bin/gnuplot.exe"  // gnuplot.exe のある場所. PROGRA~1 は Program Files を意味する.
#define STRLN 100                                    // 文字列長の上限.
#define SIZE  100                                    // 2次元自由空間の規模.
#define INFTY 100                                    // 無限大の代替値.
#define ZERO 0.01                                    // 零の代替値.
#define COULOMB 3                                    // 疑似クーロン定数.

int spc_ex(double x, double y) {                                                              // 自由空間なら0, 障害物なら1を返戻.
    if((x > 0) && (x < 100)) {                                                                // 自由空間は水平座標0と100の間.
        if(y <   0) return 1;                                                                 // 垂直座標0未満はすべて障害物.
        if(y ==  0) return  (((x <= 20) || (x >= 40))? 1 : 0);                                // 水平座標20と40に障害物があるのは
        if(y <  50) return  (((x == 20) || (x == 40))? 1 : 0);                                // 垂直座標50まで.
        if(y == 50) return ((((x >= 20) && (x <= 40)) || ((x >= 60) && (x <= 80))) ? 1 : 0);  // 垂直座標50からは,
        if(y < 100) return  (((x == 60) || (x == 80))? 1 : 0);                                // 水平座標60と80に障害物があるのは
        if(y ==100) return  (((x <= 60) || (x >= 80))? 1 : 0);                                // 垂直座標100まで.
    }                                                                                         // 水平座標0と100の間以外は
    return 1;                                                                                 // すべて障害物.
}                                                                                             // 関数終了.

int spcmap(char *fspc, int (*spc)(double, double)) {                                             // 自由空間をファイルに記述.
    int x, y, t;                                                                                 // 整数座標用変数等.
    FILE *fp; if((fp = fopen(fspc, "w")) == NULL) { printf("ERR\n"); exit(1); }                  // ファイルを開く.
    for(y = 0; y <= SIZE; y++) {                                                                 // 整数座標上を走査.
        for(x = 0; x <= SIZE; x++) if((t = spc(x, y)) == 1) fprintf(fp, "%d,%d,%d\n", x, y, t);  // 自由空間なら0を書込み.
        fprintf(fp, "\n");                                                                       // gnuplot仕様上の改行.
    }                                                                                            // 繰返しはここまで.
    return fclose(fp);                                                                           // ファイルを閉じる.
}                                                                                                // 関数終了.

int gnuplot_spc(char *fspc) {                                                      // gnuplotで自由空間を描画.
    char t[STRLN];                                                                 // ファイル名用領域確保.
    FILE *gp; if((gp = _popen(GNPLT, "w")) == NULL) { printf("ERR\n"); exit(1); }  // 制御権をgnuplotへ.
    fprintf(gp, "set size square\nset colorsequence classic\n");                   // 正方形状. 色型選択.
    fprintf(gp, "set style l 1 lt 1 lc 1 lw 1 pt 5 ps 1\n");                       // 線様式1にて線種等設定.
    fprintf(gp, "set ticscale 0\nset xtics 10\nset ytics 10\n");                   // 目盛を10ごとに設定.
    fprintf(gp, "set datafile separator ','\n");                                   // データ識別子はコンマ.
    fprintf(gp, strcat(strcat(strcpy(t, "p './"), fspc), "' ls 1 notitle\n"));     // 線様式1にて無表題描画.
    fflush(gp); fprintf(gp, "set term windows\nset output\n");                     // 出力先をWindowsへ設定.
system("pause"); fprintf(gp, "exit\n");                                            // キー入力を待って
    return _pclose(gp);                                                            // 制御権を戻す.
}                                                                                  // 関数終了.

double _pot(double x0, double y0, double x1, double y1) {               // (x0, y0)と(x1, y1)の間の疑似クーロン力を算出.
    double dx = x1 - x0, dy = y1 - y0, r = sqrt(dx * dx + dy * dy), t;  // 水平方向へdx, 垂直方向へdy離れたユークリッド距離.
    if((r > 0) && ((t = COULOMB / r) < INFTY)) return t;                // その距離にある電荷による疑似クーロン力(斥力).
    return INFTY;                                                       // ただしINFTYを上限とする.
}                                                                       // 関数終了.

double pot(int (*spc)(double, double), double xg, double yg, double x, double y) {  // 目標(xg,yg)のポテンシャルを算出する関数.
    int x0, y0;                                                                     // 整数座標用変数設定.
    double t, s = sqrt((t = xg - x) * t + (t = yg - y) * t);                        // どの位置からも目標へ一定の吸引力.
    for(y0 = 0; y0 <= SIZE; y0++)                                                   // 垂直方向へ走査.
for(x0 = 0; x0 <= SIZE; x0++)                                                       // 水平方向へ走査.
if(spc(x0, y0) == 1) s += _pot(x0, y0, x, y);                                       // 障害物なら疑似クーロン力を加算.
    return s;                                                                       // 吸引力と疑似クーロン力の合計を返戻.
}                                                                                   // 関数終了.

int potfld(char *fpot, int (*spc)(double, double), double xg, double yg) {                    // ポテンシャル場をファイルに記録.
    int x, y;                                                                                 // 整数座標用変数設定.
    FILE *fp; if((fp = fopen(fpot, "w")) == NULL) { printf("ERR\n"); exit(1); }               // ファイルを開く.
    for(y = 0; y <= SIZE; y++) {                                                              // 整数座標を走査.
        printf("%d ", y);                                                                     // 垂直座標を進捗指標として表示.
        for(x = 0; x <= SIZE; x++) fprintf(fp, "%d,%d,%f\n", x, y, pot(*spc, xg, yg, x, y));  // ポテンシャルを書込み.
        fprintf(fp, "\n");                                                                    // gnuplot仕様上の改行.
    }                                                                                         // 繰返しはここまで.
    return fclose(fp);                                                                        // ファイルを閉じる.
}                                                                                             // 関数終了.

int gnuplot_pot(char *fpot) {                                                            // gnuplotでポテンシャル場を描画.
    char t[STRLN];                                                                       // ファイル名用領域確保.
    FILE *gp; if((gp = _popen(GNPLT, "w")) == NULL) { printf("ERR\n"); exit(1); }        // 制御権をgnuplotへ.
    fprintf(gp, "set pm3d map\nset size square\nset colorsequence classic\n");           // 正方形状. 色型選択.
    fprintf(gp, "set style l 1 lt 1 lc 1 lw 1 pt 0 ps 1\n");                             // 線様式1にて線種や点種等設定.
    fprintf(gp, "set ticscale 0\nset xtics 10\nset ytics 10\nunset ztics\n");            // 目盛を10ごとに設定.
    fprintf(gp, "unset cbtics\nunset colorbox\n");                                       // 色指標は非表示.
    fprintf(gp, "set datafile separator ','\n");                                         // データ識別子はコンマ.
    fprintf(gp, strcat(strcat(strcpy(t, "sp './"), fpot), "' u 1:2:3 ls 1 notitle\n"));  // 線様式1にて無表題描画.
    fflush(gp); fprintf(gp, "set term windows\nset output\n");                           // 出力先をWindowsへ設定.
system("pause"); fprintf(gp, "exit\n");                                                  // キー入力を待って
    return _pclose(gp);                                                                  // 制御権を戻す.
}                                                                                        // 関数終了.

int frcfld(char *ffrc, int (*spc)(double, double), double xg, double yg) {             // ポテンシャル法下の力場をファイルに記録.
    int x, y;                                                                          // 整数座標用変数設定.
    double xfrc, yfrc, frc, t;                                                         // 力の各成分用変数等.
    FILE *fp; if((fp = fopen(ffrc, "w")) == NULL) { printf("ERR\n"); exit(1); }        // ファイルを開く.
    for(y = 1; y < SIZE; y++) {                                                        // 垂直方向へ走査.
        printf("%d ", y);                                                              // 垂直座標を進捗指標として表示.
        for(x = 1; x < SIZE; x++) {                                                    // 水平方向へ走査.
            if((fabs(x - xg) >= 1) || (fabs(y - yg) >= 1)) {                           // 目標に近いとき
                xfrc = pot(*spc, xg, yg, x - 0.5, y) - pot(*spc, xg, yg, x + 0.5, y);  // ポテンシャルを水平方向へ偏微分.
                yfrc = pot(*spc, xg, yg, x, y - 0.5) - pot(*spc, xg, yg, x, y + 0.5);  // ポテンシャルを垂直方向へ偏微分.
                xfrc *= 1 / (frc = sqrt(xfrc * xfrc + yfrc * yfrc));                   // 正規化し力の水平成分を算出.
                yfrc *= 1 / frc;                                                       // 正規化し力の垂直成分を算出.
                fprintf(fp, "%d,%d,%f,%f\n", x, y, xfrc, yfrc);                        // 力の各方向成分を書き込む.
            }                                                                          // 目標に近くないときは何もせず.
        }                                                                              // 水平方向の繰返しはここまで.
    }                                                                                  // 垂直方向の繰返しはここまで.
    return fclose(fp);                                                                 // ファイルを閉じる.
}                                                                                      // 関数終了.

int gnuplot_frc(char *fspc, char *ffrc) {                                                            // gnuplotで力場を描画.
    char t[STRLN];                                                                                   // ファイル名用領域確保.
    FILE *gp; if((gp = _popen(GNPLT, "w")) == NULL) { printf("ERR\n"); exit(1); }                    // 制御権をgnuplotへ.
    fprintf(gp, "set size square\nset colorsequence classic\n");                                     // 正方形状. 色型選択.
    fprintf(gp, "set style l 1 lt 1 lc 1 lw 1 pt 5 ps 1\n");                                         // 線様式1にて線種等設定.
    fprintf(gp, "set style arrow 1 head size graph 0.01,15 nofilled front lt 3 lw 1\n");             // 矢様式1にて形状等設定.
    fprintf(gp, "set ticscale 0\nset xtics 10\nset ytics 10\n");                                     // 目盛を10ごとに設定.
    fprintf(gp, "set datafile separator ','\n");                                                     // データ識別子はコンマ.
    fprintf(gp, strcat(strcat(strcpy(t, "p './"), fspc), "' ls 1 notitle\n"));                       // 線様式1にて無表題描画.
    fprintf(gp, strcat(strcat(strcpy(t, "rep './"), ffrc), "' u 1:2:3:4 w vector as 1 notitle\n"));  // 矢様式1にて無表題描画.
    fflush(gp); fprintf(gp, "set term windows\nset output\n");                                       // 出力先をWindowsへ設定.
system("pause"); fprintf(gp, "exit\n");                                                              // キー入力を待って
    return _pclose(gp);                                                                              // 制御権を戻す.
}                                                                                                    // 関数終了.

int pathmap(char *fpath, int (*spc)(double, double), double xg, double yg, double xs, double ys) {  // 経路をファイルに記録.
    int c = 0;                                                                                      // 繰返し回数用の変数設定.
    double x = xs, y = ys, xgap, ygap, xnext, ynext, xfrc, yfrc, t;                                 // 整数非限定座標用の変数設定.
    FILE *fp; if((fp = fopen(fpath, "w")) == NULL) { printf("ERR\n"); exit(1); }                    // ファイルを開く.
    while((fabs(xgap = xg - x) >= 1) || (fabs(ygap = yg - y) >= 1)) {                               // 目標に近づくまで繰り返戻.
        printf("%d ", c++);                                                                         // 繰返し回数を進捗指標で表示.
        xfrc = pot(*spc, xg, yg, x-0.5, y) - pot(*spc, xg, yg, x+0.5, y);                           // ポテンシャルを水平偏微分.
        yfrc = pot(*spc, xg, yg, x, y-0.5) - pot(*spc, xg, yg, x, y+0.5);                           // ポテンシャルを垂直偏微分.
        if((fabs(xfrc) < ZERO ) && (fabs(yfrc) < ZERO)) {                                           // 力の水平垂直成分が零のとき
            printf("DEADLOCK\n"); return fclose(fp);                                                // 斥力と吸引力の拮抗膠着.
        } else {                                                                                    // 力が非零のとき
            xnext = x + (xfrc /= (t = sqrt(xfrc * xfrc + yfrc * yfrc)));                            // 正規化し次時点水平座標算出.
            ynext = y + (yfrc /= t);                                                                // 正規化し次時点垂直座標算出.
            fprintf(fp, "%f,%f,%f,%f\n", x, y, xfrc, yfrc);                                         // 移動の両成分を書き込む.
            x = xnext;                                                                              // 水平座標を次時点に更新.
            y = ynext;                                                                              // 垂直座標を次時点に更新.
        }                                                                                           // 力が非零のときはここまで.
    }                                                                                               // 繰返しはここまで.
    if((xgap != 0) || (ygap != 0)) fprintf(fp, "%f,%f,%f,%f\n", x, y, xgap, ygap);                  // 目標までの最終移動を書込み.
    return fclose(fp);                                                                              // ファイルを閉じる.
}                                                                                                   // 関数終了.

int gnuplot_path(char *fspc, char *ffrc, char *fpath) {                                               // gnuplotで経路を描画.
    char t[STRLN];                                                                                    // ファイル名用領域確保.
    FILE *gp; if((gp = _popen(GNPLT, "w")) == NULL) { printf("ERR\n"); exit(1); }                     // 制御権をgnuplotへ.
    fprintf(gp, "set size square\nset colorsequence classic\n");                                      // 正方形状. 色型選択.
    fprintf(gp, "set style l 1 lt 1 lc 1 lw 1 pt 5 ps 1\n");                                          // 線様式1で線種等設定.
    fprintf(gp, "set style arrow 1 head size graph 0.01,15 nofilled front lt 3 lw 1\n");              // 矢様式1で形状等設定.
    fprintf(gp, "set style arrow 2 nohead size graph 0.01,15 nofilled front lc -1 lw 2\n");           // 矢様式2で形状等設定.
    fprintf(gp, "set ticscale 0\nset xtics 10\nset ytics 10\n");                                      // 目盛を10ごとに設定.
    fprintf(gp, "set datafile separator ','\n");                                                      // データ識別子はコンマ.
    fprintf(gp, strcat(strcat(strcpy(t, "p './"  ), fspc ), "' ls 1 notitle\n"));                     // 線様式1にて無表題描画.
    fprintf(gp, strcat(strcat(strcpy(t, "rep './"), ffrc ), "' u 1:2:3:4 w vector as 1 notitle\n"));  // 矢様式1にて無表題描画.
    fprintf(gp, strcat(strcat(strcpy(t, "rep './"), fpath), "' u 1:2:3:4 w vector as 2 notitle\n"));  // 矢様式2にて無表題描画.
    fflush(gp); fprintf(gp, "set term windows\nset output\n");                                        // 出力先をWindowsへ設定.
system("pause"); fprintf(gp, "exit\n");                                                               // 制御権を戻す.
    return _pclose(gp);                                                                               // 関数終了.
}


int main(void) {                                                              // メインプログラム開始.
    double xs, ys, xg, yg;                                                    // 出発と目標の座標用変数設定.
    char fspc[] = "space.dat";                                                // 自由空間のファイル名を設定.
    char fpot[] = "poten.dat";                                                // ポテンシャル場のファイル名を設定.
    char ffrc[] = "force.dat";                                                // 力場のファイル名を設定.
    char fpath[] = "path.dat";                                                // 経路のファイル名を設定.
    spcmap(fspc, spc_ex); gnuplot_spc(fspc);                                  // 自由空間を算出し描画.
    xg = 90; yg = 50;                                                         // 出発座標を(90, 50)とする.
    potfld(fpot, spc_ex, xg, yg); gnuplot_pot(fpot);                          // ポテンシャル場を算出し描画.
    frcfld(ffrc, spc_ex, xg, yg); gnuplot_frc(fspc, ffrc);                    // 力場を算出し描画.
    xs = 10; ys = 10;                                                         // 目標座標を(10, 10)とする.
    pathmap(fpath, spc_ex, xg, yg, xs, ys); gnuplot_path(fspc, ffrc, fpath);  // 経路を算出し描画.
    return 0;                                                                 // 正常終了時0を返戻.
}                                                                             // メインプログラム終了.
