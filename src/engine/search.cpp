#include "search.hpp"
#include <chess/attack.hpp>
#include <chess/bitboard.hpp>
#include <chess/makemove.hpp>
#include <chess/move.hpp>
#include <chess/movegen.hpp>
#include <chess/position.hpp>
#include <chess/raycast.hpp>
#include "time.hpp"

namespace search {

const int material[] = {100, 300, 325, 500, 900, 0};
const int passers[] = {0, 20, 20, 32, 56, 92, 140, 0};

[[nodiscard]] int eval(chess::Position &pos) {

    // Tempo bonus
    int score = 10;

    for (int c = 0; c < 2; ++c) {
        chess::Bitboard pawns[2];
        for (int c2 = 0; c2 < 2; ++c2) {
            pawns[c2] = pos.colour[c2] & pos.pieces[static_cast<int>(chess::Piece::Pawn)];
        }

        for (int p = 0; p < 6; ++p) {
            auto copy = pos.colour[0] & pos.pieces[p];
            while (copy) {
                const auto sq = chess::lsbll(copy);
                copy &= copy - 1;

                // Centrality
                const int rank = sq >> 3;
                const int file = sq & 7;
                const int centrality = -std::abs(7 - rank - file) - std::abs(rank - file);
                score += centrality * (6 - p);

                // Pawn eval
                if (p == static_cast<int>(chess::Piece::Pawn)) {
                    const auto bb = 1ULL << sq;

                    // Passed pawns
                    auto attack = chess::nw(bb) | chess::ne(bb);
                    for (auto i = 0; i < 4; ++i) {
                        attack |= chess::north(attack);
                    }
                    const auto is_passed = (attack & pawns[1]) == 0;
                    if (is_passed) {
                        score += passers[rank];
                    }
                } else if (p == static_cast<int>(chess::Piece::Rook)) {

                    // Open and semi-open files
                    const auto file_bb = 0x101010101010101ULL << file;
                    if ((file_bb & pawns[0]) == 0) {
                        if ((file_bb & pawns[1]) == 0) {
                            score += 5;
                        }
                        score += 5;
                    }

                    // Bonus on 7th/8th rank
                    if (rank >= 6) {
                        score += 15;
                    }
                }

                // Material
                score += material[p];
            }
        }

        chess::flip(pos);
        score = -score;
    }

    return score;
}

int alphabeta(chess::Position &pos,
              int alpha,
              const int beta,
              int depth,
              const int ply,
              const long long int stop_time,
              chess::Move *pvline) {
    const int ksq = chess::lsbll(pos.colour[0] & pos.pieces[static_cast<int>(chess::Piece::King)]);
    const auto in_check = chess::attacked(pos, ksq, true);

    // In-check extension
    if (in_check) {
        depth++;
    }

    const int static_eval = eval(pos);
    const bool in_qsearch = depth <= 0;
    if (in_qsearch) {
        if (static_eval >= beta) {
            return beta;
        }

        if (alpha < static_eval) {
            alpha = static_eval;
        }
    } else if (depth < 3) {

        // Reverse futility pruning
        const int margin = 120;
        if (static_eval - margin * depth >= beta) {
            return beta;
        }
    }

    // Did we run out of time?
    if (now() >= stop_time) {
        return 0;
    }

    chess::Move moves[256];
    const int num_moves = chess::movegen(pos, moves);

    int move_scores[256];
    for (int j = 0; j < num_moves; ++j) {
        auto move_score = 0;

        // PV-move first
        if (!in_qsearch && moves[j] == pvline[ply]) {
            move_score = 1 << 16;
        } else {
            // MVVLVA
            const auto capture = chess::piece_on(pos, moves[j].to);
            if (capture != chess::Piece::None) {
                move_score = ((static_cast<int>(capture) + 1) * 8) - static_cast<int>(chess::piece_on(pos, moves[j].from));
            }
        }
        move_scores[j] = move_score;
    }

    int best_score = -INF;
    for (int i = 0; i < num_moves; ++i) {

        // Pick next move
        int best_move_score = 0;
        int best_move_score_index = i;
        for (int j = i; j < num_moves; ++j) {
            if (move_scores[j] > best_move_score) {
                best_move_score = move_scores[j];
                best_move_score_index = j;
            }
        }

        const auto tempMove = moves[i];
        moves[i] = moves[best_move_score_index];
        moves[best_move_score_index] = tempMove;

        const auto tempMoveScore = move_scores[i];
        move_scores[i] = move_scores[best_move_score_index];
        move_scores[best_move_score_index] = tempMoveScore;

        // Since moves are ordered captures first, break in qsearch
        if (in_qsearch && chess::piece_on(pos, moves[i].to) == chess::Piece::None) {
            break;
        }

        auto npos = pos;

        // Check move legality
        if (!chess::makemove(npos, moves[i])) {
            continue;
        }

        // Poor man's PVS
        const int new_beta = -alpha;
        int new_alpha = -alpha - 1;
        goto do_search;
        full_search:
        new_alpha = -beta;
        do_search:
        const int score = -alphabeta(npos, new_alpha, new_beta, depth - 1, ply + 1, stop_time, pvline);
        if (score > alpha && new_alpha != -beta) {
            goto full_search;
        }

        if (score > best_score) {
            best_score = score;

            if (score > alpha) {
                alpha = score;
                pvline[ply] = moves[i];
            }
        }

        if (alpha >= beta) {
            break;
        }
    }

    // No legal moves
    if (!in_qsearch && best_score == -INF) {
        // Checkmate
        if (in_check) {
            return -MATE_SCORE;
        }
        // Stalemate
        else {
            return 0;
        }
    }

    return alpha;
}

}  // namespace search
