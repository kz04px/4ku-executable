#include "search.hpp"
#include <chess/attack.hpp>
#include <chess/bitboard.hpp>
#include <chess/makemove.hpp>
#include <chess/move.hpp>
#include <chess/movegen.hpp>
#include <chess/position.hpp>

namespace search {

// clang-format off
const int pst[6][64] = {
    {
         100, 100, 100, 100, 100, 100, 100, 100,
          99,  93,  89,  65,  87, 105, 103,  95,
         101, 101,  94,  81,  94,  93,  96, 110,
         101, 114, 108, 104, 105, 104, 110, 107,
         109, 130, 123, 131, 131, 123, 117, 111,
         121, 154, 172, 156, 177, 195, 171, 111,
         218, 221, 273, 268, 207, 182,  84, 122,
         100, 100, 100, 100, 100, 100, 100, 100,
    },{
         201, 270, 234, 236, 271, 281, 239, 219,
         244, 269, 272, 299, 293, 280, 258, 289,
         262, 284, 300, 314, 308, 303, 303, 258,
         286, 300, 302, 303, 319, 312, 333, 293,
         286, 296, 325, 333, 310, 333, 314, 343,
         278, 318, 360, 364, 424, 443, 355, 306,
         266, 324, 354, 374, 360, 422, 302, 329,
         240, 300, 300, 300, 300, 300, 300, 300,
    },{
         318, 337, 317, 288, 294, 317, 280, 258,
         340, 330, 338, 315, 326, 327, 325, 340,
         330, 337, 339, 338, 335, 324, 328, 329,
         326, 330, 348, 357, 346, 333, 342, 329,
         324, 341, 354, 352, 362, 352, 342, 329,
         332, 352, 345, 381, 416, 433, 378, 369,
         301, 302, 355, 383, 390, 386, 394, 336,
         325, 325, 325, 325, 325, 325, 325, 325,
    },{
         498, 499, 503, 501, 502, 501, 504, 492,
         474, 494, 502, 498, 502, 490, 499, 471,
         484, 500, 503, 497, 508, 499, 512, 503,
         491, 495, 508, 514, 518, 483, 513, 487,
         519, 533, 546, 557, 553, 539, 553, 516,
         524, 583, 554, 575, 634, 644, 585, 575,
         546, 533, 564, 562, 591, 589, 570, 604,
         584, 500, 500, 537, 624, 500, 500, 653,
    },{
         901, 890, 889, 903, 885, 849, 817, 887,
         893, 903, 902, 905, 899, 890, 893, 898,
         889, 900, 912, 902, 908, 911, 907, 894,
         891, 905, 907, 909, 918, 917, 926, 904,
         894, 900, 915, 925, 932, 909, 926, 912,
         884, 910, 913, 925, 937, 930, 915, 926,
         901, 911, 935, 900, 916, 955, 939, 957,
         887, 906, 858, 900, 929, 900, 900,1002,
    },{
           0,   0,   0,  -9,   0,  -9,  25,   0,
          -9,  -9,  -9,  -9,  -9,  -9,  -9,  -9,
          -9,  -9,  -9,  -9,  -9,  -9,  -9,  -9,
          -9,  -9,  -9,  -9,  -9,  -9,  -9,  -9,
          -9,  -9,  -9,  -9,  -9,  -9,  -9,  -9,
          -9,  -9,  -9,  -9,  -9,  -9,  -9,  -9,
          -9,  -9,  -9,  -9,  -9,  -9,  -9,  -9,
          -9,  -9,  -9,  -9,  -9,  -9,  -9,  -9,
    }
};
// clang-format on

[[nodiscard]] int eval(const chess::Position &pos) {
    int score = 0;

    for (int c = 0; c < 2; ++c) {
        for (int p = 0; p < 6; ++p) {
            auto copy = pos.colour[c] & pos.pieces[p];
            while (copy) {
                const auto sq = chess::lsbll(copy) ^ (c * 56);
                copy &= copy - 1;

                // PST
                score += pst[p][sq];
            }
        }

        score = -score;
    }

    return score;
}

[[nodiscard]] int search(const chess::Position &pos,
                         int alpha,
                         const int beta,
                         const int depth,
                         StackData *ss,
                         const timepoint stop_time) {
    if (depth == 0) {
        return eval(pos);
    }

    // Did we run out of time?
    if (now() >= stop_time) {
        return 0;
    }

    chess::Move moves[256];
    const int num_moves = chess::movegen(pos, moves);
    int best_score = -inf;

    // MVV-LVA
    auto score_move = [&pos, ss](const chess::Move &move) -> int {
        if (move == ss->pv) {
            return 10000;
        } else {
            const int order[] = {1, 2, 2, 3, 4, 5, 0};
            const auto captured = chess::piece_on(pos, move.to);
            return 8 * order[static_cast<int>(captured)] - order[static_cast<int>(move.place_piece)];
        }
    };

    for (int i = 0; i < num_moves; ++i) {
        // Find the best upcoming move
        for (int j = i; j < num_moves; ++j) {
            if (score_move(moves[j]) > score_move(moves[i])) {
                std::swap(moves[j], moves[i]);
            }
        }

        auto npos = pos;

        // Check move legality
        if (!chess::makemove(npos, moves[i])) {
            continue;
        }

        const auto score = -search(npos, -beta, -alpha, depth - 1, ss + 1, stop_time);

        if (score > best_score) {
            best_score = score;
            ss->pv = moves[i];

            if (score > alpha) {
                alpha = score;
            }
        }

        if (alpha >= beta) {
            break;
        }
    }

    // No legal moves
    if (best_score == -inf) {
        const auto ksq = chess::lsbll(pos.colour[0] & pos.pieces[static_cast<int>(chess::Piece::King)]);
        const auto in_check = chess::attacked(pos, ksq, true);

        // Checkmate
        if (in_check) {
            return -mate_score;
        }
        // Stalemate
        else {
            return 0;
        }
    }

    return best_score;
}

}  // namespace search
