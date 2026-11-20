//
// Created by daret on 20.11.2026.
//

#pragma once
#include <atomic>
#include <format>
#include <iostream>
#include <random>

static std::random_device rd;

class SplittableHash {
    uint64_t seed_; uint64_t gamma_;

    SplittableHash(uint64_t seed, uint64_t gamma) : seed_(seed), gamma_(gamma) {};

    uint64_t NextSeed() {
        return (seed_ += gamma_);
    }

    static uint64_t Mix64(uint64_t z) {
        z = (z ^ (z >> 33)) * 0xff51afd7ed558ccdL;
        z = (z ^ (z >> 33)) * 0xc4ceb9fe1a85ec53L;
        return z ^ (z >> 33);
    }
    static uint64_t Mix64Variant13(uint64_t z) {
        z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9L;
        z = (z ^ (z >> 27)) * 0x94d049bb133111ebL;
        return z ^ (z >> 31);
    }
    static uint64_t MixGamma(uint64_t z) {
        z = Mix64Variant13(z) | 1L;
        int n = std::popcount(z ^ (z >> 1));
        if (n >= 24) z ^= 0xaaaaaaaaaaaaaaaaL;
        return z;
    }
    static double double_ulp_;
    static uint64_t golden_gamma_;
    static std::atomic<uint64_t> default_gen_;

public:
    explicit SplittableHash(uint64_t seed) : SplittableHash(seed, golden_gamma_) {};

    explicit SplittableHash() {
        uint64_t s = default_gen_.fetch_add(2*golden_gamma_);
        this->seed_ = Mix64(s);
        this->gamma_ = MixGamma(s + golden_gamma_);
    }

    uint64_t NextLong() {
        return Mix64(NextSeed());
    }
};

double SplittableHash::double_ulp_ = 1.0 / (1L << 53);
uint64_t SplittableHash::golden_gamma_ = 0x9e3779b97f4a7c15L;
std::atomic<uint64_t> SplittableHash::default_gen_ = std::atomic<uint64_t>(rd());

static SplittableHash splittable_hash;

struct Color {
    static constexpr auto kReset = "\033[0m";
    static constexpr auto kRed = "\033[31m";
    static constexpr auto kGreen = "\033[32m";
    static constexpr auto kYellow = "\033[33m";
    static constexpr auto kBlue = "\033[34m";
    static constexpr auto kMagenta = "\033[35m";
    static constexpr auto kCyan = "\033[36m";
};

static const char* GetColor(uint64_t x) {
    switch (x%6) {
        case 0: return Color::kBlue;
        case 1: return Color::kCyan;
        case 2: return Color::kGreen;
        case 3: return Color::kMagenta;
        case 4: return Color::kRed;
        case 5: return Color::kYellow;
        default: ;
    }
    __builtin_unreachable();
}

static const char* GetColorRandom() {
    return GetColor(splittable_hash.NextLong());
}

inline void PrintRainbowText(const std::string& msg) {
    for (char ch : msg) {
        if (std::isspace(static_cast<unsigned char>(ch))) {
            std::cerr << ch;
            continue;
        }
        if ((static_cast<unsigned char>(ch) & 0xC0) != 0x80) {
            std::cerr << GetColorRandom();
        }
        std::cerr << ch;
    }
    std::cerr << Color::kReset;
}

#ifdef ENABLE_MEM_ERRORS
inline const char* GetComment() {
    static constexpr const char* kComment[] = {
        "Давай, Кирилл, ты сможешь!",
        "Не сегодня.",
        "Даже не близко.",
        "Segfault не задел.",
        "Ищи-ищи ещё.",
        "Хорошая попытка, но нет.",
        "Холодно.",
        "Горячо... нет, всё равно холодно.",
        "Кирилл, МИМО.",
        "Ошибка усть, бага - нет.",
        "Почти! (На самом деле нет.)",
        "А ты хорош. Но не настолько.",
        "Не в этот раз.",
        "Указатель на месте, и данные тоже.",
        "Valgrind скучает.",
        "ASan зевнул и пошёл дальше.",
        "Снова мимо кассы.",
        "Кирилл, попробуй ещё раз.",
        "Слишком предсказуемо.",
        "Баг не найден, зато ты увидел эту красоту.",
        "Тёпленько... ладно, заморозка.",
        "Никакого UB, одни эмоции.",
        "Куча жива, тестировщик - не факт.",
        "Статус: Кирилл старается...",
        "Ещё один смелый аргумент! В никуда...",
        "А память то течёт, течёт но не капает.",
        "Ну давай, удиви.",
        "Опять не сломалось, жалко(((",
        "Гипотеза отклонена.",
        "Сигнал 11 сегодня в отпуске.",
        "Флаг не тот, Кирилл, но мы ценим.",
        "Защита сработала, парс - нет.",
        "Это фича, а не твой шанс.",
        "Ближе к правде, но всё ещё далеко.",
        "Кирилл против C++: 0:1.",
        "Не пробило.",
        "Ошибку вывели, совесть чиста.",
        "Попробуй покрутить ещё пару флагов.",
        "Кирилл, давай ещё",
        "Мимо, но за упорство лайк.",
        "Счётчик попыток растёт 999...",
        "Ни одного упавшего потока.",
        "Кирилл, ты хоть знаешь куда лезешь?",
        "Опять мимо.",
        "Потоки отработали, ты не успел.",
        "Синхронизация прошла, поытка - нет.",
        "Ошибку поймал за хвост, но она ящерица.",
        "Кирилл, я в тебя верю (нет).",
        "Нажми ещё раз, вдруг повезёт.",
        "Архив целый, Кирилл — в шоке.",
        "Байт на месте, баг — в твоих фантазиях.",
        "Краш-тест отменён по причине несостоятельности.",
        "Не сегодня, дружище.",
        "Кремень, а не код.",
        "Снова упёрся в проверку.",
        "Держи ошибку и не плачь(хотя может и поплачь).",
        "Кирилл, вдох - выдох - вдох - выдох.",
        "Ни единого шанса на core dump.",
        "Счётчик боли фолов: ++1.",
        "Мимо, но за старание держи +0.",
        "Искал дыру, нашёл заглушку."
    };
    return kComment[splittable_hash.NextLong() % std::size(kComment)];
}
#endif

inline void PrintError(const std::string& msg) {
#ifdef ENABLE_MEM_ERRORS
    PrintRainbowText(msg);
    std::cerr << std::endl << std::endl;
    PrintRainbowText(GetComment());
    std::cerr << std::endl;
#else
    std::cerr << GetColorRandom() << msg << Color::kReset;
#endif
}

template <typename ... ARGS>
void PrintError(std::format_string<ARGS...> fmt, ARGS&&... args) {
    std::string msg = std::format(fmt, std::forward<ARGS>(args)...);
    PrintError(msg);
}