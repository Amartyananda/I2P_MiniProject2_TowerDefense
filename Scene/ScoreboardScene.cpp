#include "ScoreboardScene.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>  
#include <ctime>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"

constexpr int slotH    = 56;   // height of an entry “slot”
constexpr int gapY     = 12;   // time above centre, date below centre
constexpr int baseTop  = 240;  // Y of the first slot
constexpr int colTime  = 380;  // X positions of the three columns
constexpr int colName  = 620;
constexpr int colScore = 1225;  // right-aligned

void ScoreboardScene::LoadScores() {
    std::ifstream fin("Resource/scoreboard.txt");     // adjust path if needed
    entries.clear();

    std::string name; int sc; std::time_t ts;
    while (fin >> name >> sc >> ts) entries.push_back({ name, sc, ts });
    fin.close();
    std::sort(entries.begin(), entries.end(),
              [](const Entry& a, const Entry& b) {
                  if (a.score != b.score) return a.score > b.score;
                  else return a.ts < b.ts;          // tie-breaker: earlier first
              });
}

static std::pair<std::string, std::string> formatDateTime(std::time_t ts){
    std::tm tm = *std::localtime(&ts);
    std::ostringstream oss1, oss2;
    oss1 << std::put_time(&tm, "%H:%M");     // 24-hour clock
    oss2 << std::put_time(&tm, "%d/%m/%y");  // day/month/2-digit-year
    return { oss1.str(), oss2.str() };
}

void ScoreboardScene::Initialize() {
    auto scr   = Engine::GameEngine::GetInstance().GetScreenSize();
    int  halfW = scr.x / 2;

    labelObjs.clear();
    page = 0;
    LoadScores();
    AddNewObject(new Engine::Label("Scoreboard", "pirulen.ttf", 72, halfW, 170, 255,255,255,255, 0.5,0.5));

    auto* btnBack = new Engine::ImageButton("buttons/back.png", "buttons/back_hover.png", scr.x - 150, 50, 100, 100);
    btnBack->SetOnClickCallback(std::bind(&ScoreboardScene::BackOnClick, this));
    AddNewControlObject(btnBack);

    auto* btnPrev = new Engine::ImageButton("scoreboard/prev_button.png", "scoreboard/prev_button_hovered.png", 150, scr.y/2 - 45, 70, 90);
    btnPrev->SetOnClickCallback(std::bind(&ScoreboardScene::PagePrevOnClick, this));
    AddNewControlObject(btnPrev);

    auto* btnNext = new Engine::ImageButton("scoreboard/next_button.png", "scoreboard/next_button_hovered.png", scr.x - 240, scr.y/2 - 45, 70, 90);
    btnNext->SetOnClickCallback(std::bind(&ScoreboardScene::PageNextOnClick, this));
    AddNewControlObject(btnNext);

    bgmInstance = AudioHelper::PlaySample("scoreboard.mp3", true,AudioHelper::BGMVolume);

    RenderPage();
}

void ScoreboardScene::RenderPage() {
    for (auto* o : labelObjs) RemoveObject(o->GetObjectIterator());
    labelObjs.clear();

    constexpr int perPage = 8;
    int start = page * perPage;
    int end   = std::min<int>(entries.size(), start + perPage);

    auto scr   = Engine::GameEngine::GetInstance().GetScreenSize();
    int  halfW = scr.x / 2;

    for (int i = start; i < end; ++i) {
        const auto& e = entries[i];
        auto [tStr, dStr] = formatDateTime(e.ts);

        int topY  = baseTop + (i - start) * slotH;   // top of slot
        int midY  = topY + slotH / 2;                // vertical centre

        // time / date (two rows, small gap; considered one column)
        auto* labT = new Engine::Label(tStr, "pirulen.ttf", 22,
                                       colTime, midY - gapY,
                                       200,255,255,255, 0.0,0.5);
        auto* labD = new Engine::Label(dStr, "pirulen.ttf", 22,
                                       colTime, midY + gapY,
                                       200,255,255,255, 0.0,0.5);

        // player name – centred in slot
        auto* labN = new Engine::Label(e.name, "pirulen.ttf", 30,
                                       colName, midY,
                                       255,255, 0,255, 0.0,0.5);

        // score – centred, right-aligned at colScore
        auto* labS = new Engine::Label(std::to_string(e.score),"pirulen.ttf",30,
                                       colScore, midY,
                                       255,255, 0,255, 1.0,0.5);

        for (auto* p : {labT, labD, labN, labS}) {
            AddNewObject(p);
            labelObjs.push_back(p);
        }
    }
}


void ScoreboardScene::PagePrevOnClick() {
    if (page > 0) --page;
    RenderPage();
}
void ScoreboardScene::PageNextOnClick() {
    int maxPage = (entries.size() + 8 - 1) / 8 - 1;
    if (page < maxPage) ++page;
    RenderPage();
}

void ScoreboardScene::Terminate() {
    AudioHelper::StopSample(bgmInstance);
    bgmInstance.reset();
    IScene::Terminate();
}

void ScoreboardScene::BackOnClick() {
    Engine::GameEngine::GetInstance().ChangeScene("start");
}