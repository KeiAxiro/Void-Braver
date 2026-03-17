#include <iostream>
#include <string>
using namespace std;

struct Dungeon
{
    string name;
    string intro;
    string outro;
    string orb;
};

Dungeon dungeons[5] = {
    {"Dungeon of Valor and Ember",
     "Kamu memasuki benteng lava yang hancur...",
     "Api mulai meredup. Jiwa sang Warlord akhirnya bebas dari kutukan...",
     "Crimson Orb"},

    {"Dungeon of Knowledge and Wisdom",
     "Perpustakaan raksasa yang dipenuhi sihir kuno...",
     "Semua buku kembali diam. Pengetahuan kini tidak lagi berbisik...",
     "Azure Orb"},

    {"Dungeon of Whispers and Shadows",
     "Hutan gelap yang penuh bisikan misterius...",
     "Kabut menghilang perlahan. Hutan kembali sunyi...",
     "Obsidian Orb"},

    {"Dungeon of Thorns and Resilience",
     "Rawa beracun dengan duri raksasa di setiap sudut...",
     "Racun mulai menghilang. Alam mencoba hidup kembali...",
     "Emerald Orb"},

    {"Dungeon of Echoes and Destiny",
     "Kuil waktu yang realitanya telah retak...",
     "Aliran waktu kembali stabil... untuk saat ini...",
     "Radiant Orb"}};

void clearScreen()
{
    cout << string(50, '\n');
}

void enterDungeon(int dungeonID)
{

    clearScreen();

    cout << "====================================\n";
    cout << "Memasuki " << dungeons[dungeonID].name << endl;
    cout << "====================================\n\n";

    cout << dungeons[dungeonID].intro << endl;
    cout << "\nTekan ENTER untuk mulai...\n";
    cin.ignore();
    cin.get();

    for (int depth = 1; depth <= 20; depth++)
    {

        clearScreen();

        cout << "====================================\n";
        cout << "Depth : " << depth << endl;
        cout << "====================================\n";

        if (depth == 20)
        {
            cout << "Boss muncul di kedalaman ini!\n";
        }
        else
        {
            cout << "Musuh muncul!\n";
        }

        cout << "\n(Simulasi battle selesai... musuh dikalahkan)\n";

        if (depth < 20)
        {
            cout << "\nLanjut ke depth berikutnya...\n";
        }

        cout << "Tekan ENTER...\n";
        cin.ignore();
        cin.get();
    }

    // Setelah boss
    clearScreen();

    cout << "====================================\n";
    cout << "BOSS DIKALAHKAN!\n";
    cout << "====================================\n\n";

    cout << "Kamu mendapatkan : " << dungeons[dungeonID].orb << endl;

    cout << "\n--- OUTRO ---\n";
    cout << dungeons[dungeonID].outro << endl;

    cout << "\nTekan ENTER untuk kembali ke menu...\n";
    cin.ignore();
    cin.get();
}

void dungeonMenu()
{

    int choice;

    while (true)
    {

        clearScreen();

        cout << "========== DUNGEON MENU ==========\n";
        cout << "1. Dungeon of Valor and Ember\n";
        cout << "2. Dungeon of Knowledge and Wisdom\n";
        cout << "3. Dungeon of Whispers and Shadows\n";
        cout << "4. Dungeon of Thorns and Resilience\n";
        cout << "5. Dungeon of Echoes and Destiny\n";
        cout << "0. Keluar\n";
        cout << "==================================\n";

        cout << "Pilih Dungeon : ";
        cin >> choice;

        if (choice == 0)
        {
            break;
        }

        if (choice >= 1 && choice <= 5)
        {
            enterDungeon(choice - 1);
        }
    }
}

int main()
{

    dungeonMenu();

    return 0;
}