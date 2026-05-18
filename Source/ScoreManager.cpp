#include "ScoreManager.h"
#include <fstream>
#include <algorithm>
#include <filesystem>

ScoreManager::ScoreManager()
{
	// 起動時に自動でファイルを読み込む
	LoadScore();
}

void ScoreManager::RegisterScore(int score)
{
	// 最後にプレイしたスコアを保存
	lastPlayScore = score;

	// スコアを追加
	myScores.push_back(score);

	// スコアを降順にソート
	std::sort(myScores.begin(), myScores.end(), std::greater<int>());

	// 上限を超えたら古いスコアを削除
	if (myScores.size() > MaxScoresCount) {
		myScores.resize(MaxScoresCount);
	}

	// スコアを保存
	SaveScore();
}

void ScoreManager::SaveScore()
{
	// 保存先のディレクトリが存在しない場合は作成
	std::filesystem::path p(savePath);
	std::filesystem::path dir=p.parent_path();
	if (!std::filesystem::exists(dir)) {
		std::filesystem::create_directories(dir);
	}

	std::ofstream outFile(savePath, std::ios::binary);
	if (!outFile)return;

	// 件数と配列の中身をそのまま保存
	size_t size = myScores.size();
	outFile.write(reinterpret_cast<const char*>(&size), sizeof(size));
	if (size > 0) {
		outFile.write(reinterpret_cast<const char*>(myScores.data()), size * sizeof(int));
	}
}

void ScoreManager::LoadScore()
{
	myScores.clear();
	std::ifstream inFile(savePath, std::ios::binary);
	if (!inFile)return; // ファイルが存在しない場合は空のスコアリストを返す

	// 件数と配列の中身を読み込む
	size_t size = 0;
	inFile.read(reinterpret_cast<char*>(&size), sizeof(size));
	if (size > 0) {
		myScores.resize(size);
		inFile.read(reinterpret_cast<char*>(myScores.data()), size * sizeof(int));
	}

	while(myScores.size()< MaxScoresCount) {
		myScores.push_back(0); // 足りない分は0で埋める
	}
}