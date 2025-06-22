#pragma once

#include "../system/scene.h"
#include "../system/camera.h"
#include "../system/random.h"

#include "../object/player.h"
#include "../object/enemy.h"
#include "../object/floor.h"

#include "../component/sprite.h"
#include "../component/button.h"

#include <random>

/// <summary>
/// 게임 상태
/// </summary>
enum GameState { Playing, Menu, GameOver };

/// <summary>
/// 업그레이드 종류
/// </summary>
enum UpgradeType { Health, Speed, Interval };

const float HEALTH_COEFF = 20.0f;				// 업그레이드로 인한 체력 회복량
const float ENEMY_SPAWN_COOLDOWN = 10.0f;		// 적 스폰 시간
const float INITIAL_COOLDOWN_MODIFIER = 3.0f;	// 처음 적 스폰 시간 조정 값
const int INITIAL_ENEMY_COEFF = 5;				// 처음 적 생성 값
const int ENEMY_COEFF_INCREASEMENT = 3;			// 단계별 적 생성 값 증가량

/// <summary>
/// 인게임 씬
/// </summary>
class InGame : public Scene {
public:
	InGame(string);
	void Initialize();

	/// <summary>
	/// 현재 게임 상태를 반환한다.
	/// </summary>
	/// <returns>게임 상태</returns>
	GameState GetGameState();

private:
	void Start();
	void Frame();

private:
	GameState m_gameState;

	Player* m_player;

	Button* m_resumeButton;
	Button* m_quitButton;

	Transform* m_healthBar;
	Transform* m_healthBarBackground;

	Button* m_firstUpgradeButton;
	Button* m_secondUpgradeButton;

	Sprite* m_firstUpgradeSprite;
	Sprite* m_secondUpgradeSprite;

	UpgradeType m_firstUpgradeType;
	UpgradeType m_secondUpgradeType;

	GameObject* m_gameOverDisplay;

	float m_upgradeCooldown;
	float m_enemySpawnCooldown;

	int m_enemySpawnCoeff;
};