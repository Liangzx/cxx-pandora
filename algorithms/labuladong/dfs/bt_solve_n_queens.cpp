#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <string>

bool isValid(std::vector<std::string> &board, int row, int col);
void backtrack(std::vector<std::string> &board, int row, std::vector<std::vector<std::string>> &results);

std::vector<std::vector<std::string>> solveNQueens(int n)
{
    // '.' 表示空，'Q' 表示皇后，初始化空棋盘
    std::vector<std::string> board(n, std::string(n, '.'));

    std::vector<std::vector<std::string>> results;

    backtrack(board, 0, results);
}

// 路径: board 小于 row 的那些行已经成功放置了皇后
// 选择列表：第 row 行的所有列都是放置皇后的选择
// 结束：row 超过 board 的最后一行
void backtrack(std::vector<std::string> &board, int row, std::vector<std::vector<std::string>> &results)
{
    // 触发结束条件
    if (row == board.size())
    {
        results.push_back(board);
        return;
    }
    //
    int n = board[row].size();
    //
    for (int col = 0; col < n; col++)
    {
        // 判定是否有效
        if (!isValid(board, row, col))
        {
            continue;
        }

        // 做选择
        board[row][col] = 'Q';

        // 进入下一行决策
        backtrack(board, row + 1, results);

        // 撤销选择
        board[row][col] = '.';
    }
}

//
bool isValid(std::vector<std::string> &board, int row, int col)
{
    int n = board[row].size();

    // 检查列是否有皇后互相冲突
    for (int i = 0; i < n; i++)
    {
        if (board[i][col] == 'Q')
        {
            return false;
        }
    }

    // 检查右上方是否有皇后互相冲突
    for (int i = row - 1, j = col + 1; i >= 0 && j < n; i--, j++)
    {
        if (board[i][j] == 'Q')
        {
            return false;
        }
    }

    // 检查左上方是否有皇后互相冲突
    for (int i = row - 1, j = col - 1; i >= 0 && j >= 0; i--, j--)
    {
        if (board[i][j] == 'Q')
        {
            return false;
        }
    }

    return true;
}
