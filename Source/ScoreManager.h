#pragma once
#include <vector>
#include <string>

class ScoreManager
{
private:
	ScoreManager();
	~ScoreManager() = default;

public:
	static ScoreManager& Instance()
	{
		static ScoreManager instance;
		return instance;
	}
	
	// スコアの登録
	void RegisterScore(int score);

	// 歴代最高スコアの取得
	int GetHighScore() const { return myScores.empty() ? 0 : myScores[0];}

	// ランキングの取得
	const std::vector<int>& GetTopScores() const { return myScores; }

	// 最後にプレイしたスコアの取得
	int GetLastPlayScore() const { return lastPlayScore; }

private:
	void SaveScore();
	void LoadScore();

	std::vector<int> myScores; // プレイヤーのスコアを保存するベクター
	const int MaxScoresCount = 5; // 上位5件を保存
	const std::string savePath = "Data/Save/ranking.dat"; // スコアを保存するファイル名

	int lastPlayScore = 0; // 最後にプレイしたスコアを保存する変数
};
