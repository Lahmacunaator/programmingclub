#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>

void runGame();
int executeTurn();

void handleUpdates();
void outputState();
int handleEncounters();
void handlePlayerChoice();
void savefile();
void savefile1();
void savefile2();
void loadfile();
void loadfile1();
void loadfile2();
void printmapandlocation();
void viewinventory();
int getPlayerChoiceMenu();
void printplayerstats();
void printsuccubus();

char * getRoomText();
char * getAvailableDirections();
int handleBossEncounter();
int handleMiniBossEncounter();
int handleSuccubusEncounter();
int handlePriestEncounter();
int handleFoulEncounter();
int getPlayerChoice();
void addPlayerLives();
int handleCombatChoicevsSuccubus();
int handleCombatChoicevsBoss();
int handleCombatChoicevsMiniBoss();
int handleCombatChoicevsPriest();
int handleCombatChoicevsFoul();
int BossAppearance();
int MiniBossAppearance();
int SuccubusAppearance();
int PriestAppearance();
int FoulAppearance();
void escapeAttempt(int *combatEnded);
void Attackvaluevsboss(int *combatEnded);
void Attackvaluevssuccubus(int *combatEnded);
void AttackvaluevsMiniboss(int *combatEnded);
void AttackvaluevsPriest(int *combatEnded);
void AttackvaluevsFoul(int *combatEnded);
void BossAttackvalue(int *combatEnded);
void MiniBossAttackvalue(int *combatEnded);
void SuccubusAttackvalue(int *combatEnded);
void PriestAttackvalue(int *combatEnded);
void FoulAttackvalue(int *combatEnded);
void healsquare1();
void healsquare2();
void healsquare3();
void healsquare4();

//Room coordinates are x,y,
const int playerX = 0;
const int playerY = 0;
const int playerHealth = 150;
const int playercritchance = 30;
const int playerattackdamage = 20;
const int playercritdamagemultiplier = 2;
const int playerdefense = 5;
const int playerfailchance = 10;
const int playerhavethestaff = 0;
const int questaccepted = 0;
const int acquiredpotatopeeler = 0;

const int bossHealth = 1000;
const int bossDefense = 20;
const int bossAttack = 100;
const int bossCritChance = 10;

const int MinibossHealth = 100;
const int MinibossDefense = 6;
const int MinibossAttack = 20;
const int MinibossCritChance = 10;

const int succubuscharm = 0;
const int succubushealth = 150;
const int succubusdefense = 5;
const int succubuscritchance = 30;
const int succubusattack = 20;

const int priesthealth = 1000;
const int priestdefense = 20;
const int priestcritchance = 30;
const int priestattack = 80;

const int foulattack = 13;
const int foulhealth = 30;
const int foulrespawn = 30;

static int heal1 = 0;
static int heal2 = 0;
static int heal3 = 0;
static int heal4 = 0;

struct Player {
    int XLocation;
    int YLocation;
    int health;
    int critchance;
    int attackdamage;
    int critdamage;
    int failchance;
    int defense;
    int priestsstaff;
    int priestsquest;
    int potatopeeler;
    int succubusspell;
};

struct Boss {
    int bossHP;
    int bossDEF;
    int bossATK;
    int bossCRT; //this is crit chance, crit damage mutliplier is 2.
};

struct Miniboss{
    int MinibossHP;
    int MinibossDEF;
    int MinibossATK;
    int MinibossCRT;

};

struct Succubus{
    int succubusHP;
    int succubusATK;
    int succubusCRT;
    int succubusDEF;
};

struct Priest{
    int priestHP;
    int priestATK;
    int priestCRT;
    int priestDEF;
};
struct Foulspawn{
    int foulHP;
    int foulATK;
};

static struct Player player;
static struct Boss boss;
static struct Miniboss miniboss;
static struct Succubus succubus;
static struct Priest priest;
static struct Foulspawn foulspawn;

static int healingRegen = 0;
static int turns = 0;

static char directionNames[4][10]  = {
        "(u)p",
        "(r)ight",
        "(d)own",
        "(l)eft"
};
// x  y  direction
static int availableDirections[5][5][4]  = {
        //Definition for a room's available directions: Up, Right, Down, Left. 0 - blocked, 1 - available
        {  //x=0
                { 1, 1, 0, 0}, //y=0
                { 1 ,1, 1, 0}, //y=1
                { 1, 1, 1, 0}, //y=2
                { 1, 1, 1, 0}, //y=3
                { 0, 1, 1, 0}, //y=4
        },
        { //x=1
                { 1, 1, 0, 1}, //y=0
                { 1, 1, 1, 1}, //y=1
                { 1, 1, 1, 1}, //y=2
                { 1, 1, 1, 1}, //y=3
                { 0, 1, 1, 1}, //y=4
        },
        { //x=2
                { 1, 1, 0, 1}, //y=0
                { 1, 1, 1, 1}, //y=1
                { 1, 1, 1, 1}, //y=2
                { 1, 1, 1, 1}, //y=3
                { 0, 1, 1, 1}, //y=4
        },
        { //x=3
                { 1, 1, 0, 1}, //y=0
                { 1, 1, 1, 1}, //y=1
                { 1, 1, 1, 1}, //y=2
                { 1, 1, 1, 1}, //y=3
                { 0, 1, 1, 1}, //y=4
        },
        { //x=4
                { 1, 0, 0, 1}, //y=0
                { 1, 0, 1, 1}, //y=1
                { 1, 0, 1, 1}, //y=2
                { 1, 0, 1, 1}, //y=3
                { 0, 0, 1, 1}, //y=4
        }
};

static char roomTexts[5][5][500] = {
        //X=0
        {
                {"This is where you start your journey in the Coldharbour Insanity Dungeon, where the source of all evil is inherited.\nYou grabbed your gear rushed here when you heard the priest's help call.\nYou can go:"}, //y = 0
                {"As you go further into the dungeon you feel both darkness and light, total chaos around you.\nYou Can go:\n"}, //y = 1
                {"A little slime guy tries to pick a fight with you...\n"}, //y = 2
                {"You are in the Succubus' Premises.\n"}, //y = 3
                {"You are in the Succubus' Training Grounds\n"}, //y = 4
        },
        //X=1
        {
                {"You keep exploring into the ColdHarbour Insanity Dungeon. You Can go:\n"}, //y = 0
                {"You have a weird encounter:\n"}, //y = 1
                {"You see a beast making love to a rock... Weird demons...\n"}, //y = 2
                {"You have a weird encounter:\n"}, //y = 3
                {"You Arrived at the demon hospital. These nice demons can heal you for 200 HP once every 10 turns!\n"}, //y = 4
        },
        //X=2
        {
                {"An octopus headed guy tries to pick a fight with you...\n"}, //y = 0
                {"You can see the priest's prison up north.\n"}, //y = 1
                {"You enter the priest's magic prison\n"}, //y = 2
                {"You feel extremely chaotic as if you were stuck in between the good and evil.\n"}, //y = 3
                {"A skeleton knight(?) tries to pick a fight with you...\n"}, //y = 4
        },
        //X=3
        {
                {"You Feel a dark energy radiating through the swamp on your east.\n"}, //y = 0
                {"You smell horses:\n"}, //y = 1
                {"You see a statute of Cthulhu.\n"}, //y = 2
                {"You have a weird encounter:\n"}, //y = 3
                {"You see a field of black potatoes? What kind of things are those demons?\n"}, //y = 4
        },
        //X=4
        {
                {"You came to the swamp of the dungeon\n"}, //y = 0
                {"You see some demons trying to swim in lava: They take 1000 damage and die!\n"}, //y = 1
                {"Cthulhu's Grandson tries to pick a fight with you...\n"}, //y = 2
                {"You Visit the Demon Library of Black Books. Shhhh!\n"}, //y = 3
                {"The sign here says: Prof. Dr. DOOM POTATO.\n"}, //y = 4
        }

};
//Main Method
int main()
{

    //Initialize player base stats
    player.XLocation = playerX;
    player.YLocation = playerY;
    player.health = playerHealth;
    player.critchance = playercritchance;
    player.attackdamage = playerattackdamage;
    player.critdamage = playercritdamagemultiplier;
    player.failchance = playerfailchance;
    player.defense = playerdefense;
    player.potatopeeler = acquiredpotatopeeler;
    player.succubusspell = succubuscharm;

    //Initialize quest booleans
    player.priestsstaff = playerhavethestaff;
    player.priestsquest = questaccepted;

    //Initialize boss stats
    boss.bossHP = bossHealth;
    boss.bossDEF = bossDefense;
    boss.bossATK = bossAttack;
    boss.bossCRT = bossCritChance;

    //Initialize Mini Boss Stats
    miniboss.MinibossHP = MinibossHealth;
    miniboss.MinibossDEF = MinibossDefense;
    miniboss.MinibossATK = MinibossAttack;
    miniboss.MinibossCRT = MinibossCritChance;

    //Initialize Succubus Stats
    succubus.succubusATK = succubusattack;
    succubus.succubusDEF = succubusdefense;
    succubus.succubusHP = succubushealth;
    succubus.succubusCRT = succubuscritchance;

    //Initialize Priest Stats For Alternate Ending
    priest.priestATK = priestattack;
    priest.priestDEF = priestdefense;
    priest.priestHP = priesthealth;
    priest.priestCRT = priestcritchance;

    //Initialize "make the game harder mob" stats
    foulspawn.foulATK = foulattack;
    foulspawn.foulHP = foulhealth;

    //enter Game loop
    runGame();
    return 0;
}

void runGame(){
    printf("Press 1 for New Game\t\tPress 2 to Load Game\n");
    int option = 0;
    scanf("%d", &option);

    switch(option)
    {
        case (1):
            break;
        case (2):
            int loslot = 0;
            printf("Select loading slot: (1) (2) (3)\n");
            scanf("%d", &loslot);
            switch (loslot){
                case (1):
                    loadfile();
                    break;
                case (2):
                    loadfile1();
                    break;
                case (3):
                    loadfile2();
                    break;
                default:
                    printf("Invalid Slot\a\n");
                    break;
            }
            break;
    }
    int isGameOver = 0;
    //While the game is not over, execute loop
    while(!isGameOver){
        isGameOver = executeTurn();
    }
}

int executeTurn(){
    int gameOver;
    //increment turn counter
    turns++;
    //handle any updates, that have to happen each turn
    handleUpdates();
    //output details about player's status: health, location, etc.
    outputState();
    //handle a random encounter, and check whether it caused the game to end
    gameOver = handleEncounters();
    if(gameOver > 0){
        return 1;
    }
    //give player a choice for further exploration
    handlePlayerChoice();
    return 0;
}

void handleUpdates(){
    //decrement the amount of turns left until healing is available
    if(healingRegen > 0){
        healingRegen--;
    }
}

void outputState(){
    char* roomText;
    //get room description text
    roomText = getRoomText();
    printf("\n\n\n\n");
    printf("\nPlayer location: X: %d Y: %d",player.XLocation,player.YLocation);
    printf("\nPlayer health: %d",player.health);
    printf("\nCurrent turn: %d \n",turns);
    printf("\n%s",roomText);
    printf("\n\n");
}

//Decide any encounters that should happen in the given room.
int handleEncounters(){
    switch(player.XLocation){
        case 0:
            switch(player.YLocation){
                case 0:
                    break;
                case 1:
                    break;
                case 2:
                        if (FoulAppearance()) {
                            return handleFoulEncounter();
                        }

                    break;
                case 3:
                    break;
                case 4:
                    if (player.succubusspell == 1){
                        if(SuccubusAppearance()){
                            return handleSuccubusEncounter();
                        }
                    }
                    break;
            }
            break;
        case 1:
            switch(player.YLocation){
                case 0:
                    break;
                case 1:
                    healsquare1();
                    break;
                case 2:
                    break;
                case 3:
                    healsquare3();
                    break;
                case 4:
                    if(healingRegen == 0){
                        player.health += 200;
                        printf("Crafty demonesses healed you for 200 HP!\n");
                        healingRegen += 10;
                    }
                    break;
            }
            break;
        case 2:
            switch(player.YLocation){
                case 0:
                        if (FoulAppearance()) {
                            return handleFoulEncounter();
                        }

                    break;
                case 1:
                    break;
                case 2:

                    if(player.priestsquest == 0){
                        printf("The half naked old priest approaches you with a vodka bottle in hand.\nDamned demons trapped me here with their curses!\n");
                        printf("priest: Young warrior! My magic wand is stolen by evil creature! Bring it back for me and I will reward you generously!\n");
                        printf("Accept the quest? (y)/(n)\n");
                        char yesorno;
                    scanf("%c",&yesorno);
                    if(yesorno == 'y'){
                        printf("Priest: Head South East from here child, there you will find the vile beast. May Talos bless you.\n");
                        player.priestsquest += 1;
                    }
                    if(yesorno == 'n'){
                        printf("Priest: Hmph. I will be here if you change your mind.\n");
                    }
                    }
                    if(player.priestsstaff == 1){
                        printf("The half naked old priest approaches you with a vodka bottle in hand.\nDamned demons trapped me here with their curses!");
                    addPlayerLives();
                    }
                    if(player.priestsquest == 3) {
                        if(PriestAppearance()){
                            return handlePriestEncounter();
                        }
                    }
                break;
                case 3:
                    break;
                case 4:

                        if (FoulAppearance()) {
                            return handleFoulEncounter();
                        }

                    break;
            }
            break;
        case 3:
            switch(player.YLocation){
                case 0:
                    break;
                case 1:
                    healsquare2();
                    break;
                case 2:
                    break;
                case 3:
                    healsquare4();
                    break;
                case 4:
                    break;
            }
            break;
        case 4:
            switch(player.YLocation){
                case 0:
                   if(player.priestsquest == 1){
                       if(MiniBossAppearance()){
                           return handleMiniBossEncounter();
                       }
                   }
                    break;
                case 1:
                    break;
                case 2:
                        if (FoulAppearance()) {
                            return handleFoulEncounter();
                        }
                    break;
                case 3:
                    break;
                case 4:
                    if(player.succubusspell == 0){
                    if(BossAppearance()){
                        return handleBossEncounter();
                    }
                    }
                    break;
            }
            break;
    }
    return 0;
}

void handlePlayerChoice(){
    printf("(m)enu \n");
    int choice = getPlayerChoice();
    switch(choice){
        //u - up
        case 117:
            if(availableDirections[player.XLocation][player.YLocation][0]){
                printf("\nYou chose to go up.\n\n");
                player.YLocation++;
            } else{
                printf("\nYou cannot go there");
            }
            break;
            //r - right
        case 114:
            if(availableDirections[player.XLocation][player.YLocation][1]){
                printf("\nYou chose to go right.\n\n");
                player.XLocation++;
            } else{
                printf("\nYou cannot go there");
            }
            break;
            //d - down
        case 100:
            if(availableDirections[player.XLocation][player.YLocation][2]){
                printf("\nYou chose to go down.\n\n");
                player.YLocation--;
            } else{
                printf("\nYou cannot go there");
            }
            break;
            //l - left
        case 108:
            if(availableDirections[player.XLocation][player.YLocation][3]){
                printf("\nYou chose to go left.\n\n");
                player.XLocation--;
            } else{
                printf("\nYou cannot go there");
            }
            break;
        case 109:
            int choicemenu = getPlayerChoiceMenu();
            switch(choicemenu){
                case 109:
                    printmapandlocation();
                    printf("press enter to continue\n");
                break;
                case 108:
                    int lslot = 0;
                    printf("Select loading slot: (1) (2) (3)\n");
                    scanf("%d", &lslot);
                    switch (lslot) {
                        case (1):
                            loadfile();
                            break;
                        case (2):
                            loadfile1();
                            break;
                        case (3):
                            loadfile2();
                            break;
                        default:
                            printf("Invalid Slot\a\n");
                            break;
                    }
                    printf("press enter to continue\n");
                    break;
                case 115:
                    int slot = 0;
                    printf("Select saving slot: (1) (2) (3)\n");
                    scanf("%d", &slot);
                    switch (slot){
                        case (1):
                            savefile();
                            break;
                        case (2):
                            savefile1();
                            break;
                        case (3):
                            savefile2();
                            break;
                        default:
                            printf("Invalid Slot\a\n");
                            break;
                    }
                    printf("press enter to continue\n");
                    break;
                case 105:
                    viewinventory();
                    printf("press enter to continue\n");
                    break;
                case 114:
                    printf("press enter to continue\n");
                    break;
                case 99:
                    printplayerstats();
                    printf("press enter to continue\n");
                    break;
                default:
                    printf("Unknown choice %d", choice);
                    printf("press enter to continue\n");
            }
            getchar(); //To give players a better chance to view what they looked for in menu UI FOR THE WIN
            break;
        default:
            printf("Unknown choice %d", choice);
    }
    getchar();
}

int getPlayerChoice(){
    char selected;
    char* availableDirections;
    availableDirections  = getAvailableDirections();
    printf("\n%s\nWhere will you go? Choose wisely:  ",availableDirections);
    scanf(" %c", &selected);
    return (int)selected;
}
int getPlayerChoiceMenu(){
    char selected2;
    printf("(s)ave\t(l)oad\n(m)ap\t(i)nventory\n(r)esume(c)haracter\n");
    scanf(" %c", &selected2);
    turns -= 1;
    return (int)selected2;
}

char * getRoomText(){
    static char fullString[240];
    strcpy(fullString,"");
    strcat(fullString, roomTexts[player.XLocation][player.YLocation]);
    return fullString;
}

char * getAvailableDirections(){
    static char availableDirectionString[40];
    strcpy(availableDirectionString,"");
    //Iterate through all directions, and if the value is 1, add to the player output
    for(int i = 0; i < 4; i++){
        if(availableDirections[player.XLocation][player.YLocation][i]){
            strcat(availableDirectionString, directionNames[i]);
            strcat(availableDirectionString,"\n");
        }
    }
    return availableDirectionString;
}

int BossAppearance(){
    return 1;
}
int MiniBossAppearance(){
    return 1;
}
int SuccubusAppearance() {
    return 1;
}
int PriestAppearance() {
    return 1;
}
int FoulAppearance(){
    return 1;
}

int handleBossEncounter(){
    printf("A horrible, shape-shifting figure appears in front of you.\n"
           "you figure that it is actually a potato.\n"
           "You hear his shrieking voice: \n'DOOM! Who dares to disturb the DOOM POTATO!? DOOM!'\n"
           "DOOM POTATO notices your strength and transforms into his battle form\n\n");
    Sleep(500);
    printf("He transforms into the shape of...\n");
    Sleep(300);
    printf("SHREK..?!\n\n");
    printf("Shrek(DOOM POTATO) has %d health left\n", bossHealth);
    return handleCombatChoicevsBoss();
}
int handleMiniBossEncounter(){
    printf("You see a huge swarm locusts gathering together and forming the Vile Warlock.\n");
    printf("The Vile Warlock has %d health left\n", MinibossHealth);
    printf("You can choose to run or fight. \n");
    return handleCombatChoicevsMiniBoss();
}

int handleSuccubusEncounter(){
    printsuccubus();
    Sleep(2000);
    printf("You see the most charming woman you have ever seen.\n"
           "She gives you your equipment back\n"
           "As you equip your gear, she comes closer to you and introduces herself as Mistress Rohini the Succubus\n"
           "She offers you to join her side, or die, as she starts casting her charm spell, she waits for your answer\n"
           "You have to fight her to stop her or be her thrall, will you fight?\n");
    return handleCombatChoicevsSuccubus();
}

int handlePriestEncounter(){
    printf("\nYou push the priest away after taking your reward.\n"
           "You tell him that you serve Mistress Rohini now\n"
           "You unsheathe your sword and get ready to kill the priest\n"
           "Priest:'Fool demon scum! Do you think you can survive against my holy magic?'\n");
    return handleCombatChoicevsPriest();
}

int handleFoulEncounter(){
    printf("\nYour battle starts after it's weird battle dance.\n");
    return handleCombatChoicevsFoul();
}

void addPlayerLives(){
        printf("The Priest rewards you with the Holy Potato Peeler.\n You feel stronger than ever before! \n");
        player.potatopeeler += 1;
        player.health += 1100;
        printf("HP INCREASED BY 1100! \n");
        player.attackdamage += 30;
        printf("ATK INCREASED BY 30! \n");
        player.critdamage += 1;
        printf("CRIT DMG INCREASED BY 1! \n");
        player.defense += 4;
        printf("DEF INCREASED BY 4! \n");
        player.critchance += 15;
        printf("CRIT CHANCE INCREASED BY 0.15! \n");
        player.priestsstaff +=1;
        Sleep(300);
        printf("Priest: 'Our work here is not done, I located the source of darkness. \nHead northwest and be ready to face the deadliest thing you have ever seen!'\n\n");
}

int handleCombatChoicevsBoss(){
    char selected;
    int combatEnded;
    combatEnded = 0;
    printf("\n\nDo you: (r)un or (a)ttack\n");
    //Pass the MEMORY ADDRESS of the selected value to the scanf function
    scanf(" %c", &selected);
    switch(selected){
        case 'r':
            //Pass the reference to the Combat ended variable
            escapeAttempt(&combatEnded);
            break;
        case 'a':
            Attackvaluevsboss(&combatEnded);
            Sleep(200);
            BossAttackvalue(&combatEnded);
            break;
        default:
            printf("Invalid option.\n");
            combatEnded = 0;
    }

    if(player.health <= 0){
        printf("You died. 'GET OUT OF MY SWAMP! DOOM!\nGame over\n");
        return 1;
    }
    if(boss.bossHP <= 0){
        printf("You killed the source of darkness!\n The world is now free of the DOOM POTATO.\nGame over\n");
        return 1;
    }
    if(!combatEnded){
        return handleCombatChoicevsBoss();
    }
    return 0;
}
int handleCombatChoicevsMiniBoss(){
    char selected;
    int combatEnded;
    combatEnded = 0;
    if(player.priestsquest == 1){
    printf("\n\nDo you: (r)un or (a)ttack\n");
    //Pass the MEMORY ADDRESS of the selected value to the scanf function
    scanf(" %c", &selected);
    switch(selected){
        case 'r':
            //Pass the reference to the Combat ended variable
            escapeAttempt(&combatEnded);
            break;
        case 'a':
            AttackvaluevsMiniboss(&combatEnded);
            Sleep(2000);
            MiniBossAttackvalue(&combatEnded);
            break;
        default:
            printf("Invalid option.\n");
            combatEnded = 0;
    }
    }
    if(player.health <= 0){
        printf("You died!\nGame over\n");
        return 1;
    }
    if(miniboss.MinibossHP <= 0){
        printf("You defeated the Vile Warlock!\nLocusts disappear into the void as his body melts. \nYou pick up priest's staff from the ground. \n");
        player.priestsstaff += 1;
        player.priestsquest += 1;
        return 0;
    }
    if(!combatEnded){
        return handleCombatChoicevsMiniBoss();
    }
    return 0;
}

int handleCombatChoicevsPriest(){
    char selected;
    int combatEnded;
    combatEnded = 0;
    if(player.priestsquest == 3){
        printf("\n\nDo you: (r)un or (a)ttack\n");
        //Pass the MEMORY ADDRESS of the selected value to the scanf function
        scanf(" %c", &selected);
        switch(selected){
            case 'r':
                //Pass the reference to the Combat ended variable
                escapeAttempt(&combatEnded);
                break;
            case 'a':
                AttackvaluevsPriest(&combatEnded);
                Sleep(2000);
                PriestAttackvalue(&combatEnded);
                break;
            default:
                printf("Invalid option.\n");
                combatEnded = 0;
        }
    }
    if(player.health <= 0){
        printf("You died!\nGame over\n");
        return 1;
    }
    if(priest.priestHP <= 0){
        printf("You killed the priest!\nSuccubus Rohini appears and you kneel before her. \nHer voice echoes in your head as she speaks: \n'Good job thrall, now it is time to sleep... Until I need your assistance again...'\n");
        printf("Game Over! You win..?");
        return 1;
    }
    if(!combatEnded){
        return handleCombatChoicevsPriest();
    }
    return 0;
}

int handleCombatChoicevsSuccubus(){
    char selected;
    int combatEnded;
    combatEnded = 0;
    printf("\n\n(f)ight\t(s)urrender to darkness\n");
    //Pass the MEMORY ADDRESS of the selected value to the scanf function
    scanf(" %c", &selected);
    switch(selected){
        case 's':
            //Pass the reference to the Combat ended variable
            player.succubusspell += 1;
            player.priestsquest += 1;
            combatEnded = 1;
            printf("'Good my thrall, now look into my eyes and get ready to serve your new mistress'\n"
                   "You watch as she casts her charm spell and puts you back to sleep...\n");
            player.YLocation = 3;
            turns += 2;
            Sleep(300);
            printf("Succubus Rohini wakes you up at her hut.\n"
                   "With a commanding voice, she tell's you that your first task is to kill the priest.\n"
                   "You bow down and accept your task.\n");
            break;
        case 'f':
            Attackvaluevssuccubus(&combatEnded);
            Sleep(300);
            SuccubusAttackvalue(&combatEnded);
            break;
        default:
            printf("Invalid option.\n");
            combatEnded = 0;
    }

    if(player.health <= 0){
        printf("You died!\nGame over\n");
        return 1;
    }
    if(succubus.succubusHP <= 0){
        printf("You have slain the sucubus Rohini!\nYou collect her essence to greatly increase your Critical Hit Rate!\n");
        player.succubusspell -= 1;
        player.critchance += 30;
        return 0;
    }
    if(!combatEnded){
        return handleCombatChoicevsSuccubus();
    }
    return 0;
}

int handleCombatChoicevsFoul(){
    char selected;
    int combatEnded;
    combatEnded = 0;
        printf("\n\nDo you: (r)un or (a)ttack\n");
        //Pass the MEMORY ADDRESS of the selected value to the scanf function
        scanf(" %c", &selected);
        switch(selected){
            case 'r':
                //Pass the reference to the Combat ended variable
                escapeAttempt(&combatEnded);
                break;
            case 'a':
                AttackvaluevsFoul(&combatEnded);
                Sleep(300);
                FoulAttackvalue(&combatEnded);
                break;
            default:
                printf("Invalid option.\n");
                combatEnded = 0;
        }
    if(player.health <= 0){
        printf("You died!\nGame over\n");
        return 1;
    }
    if(foulspawn.foulHP <= 0){
        printf("It died? Get out of there before you get your pants dirty!!\n");
        foulspawn.foulHP = foulrespawn;
        return 0;
    }
    if(!combatEnded){
        return handleCombatChoicevsFoul();
    }
    return 0;
}

//accept a pointer to a variable
void escapeAttempt(int *combatEnded){
    int randomResult;
    time_t t;
    srand((unsigned) time(&t));
    randomResult = rand() % 6;
    if(randomResult > 1){
        printf("You have succeeded in escaping the monster without harm!\n");
        //assign a value to the variable.
        *combatEnded = 1;
    }else{
        printf("You try to run, but the monster manages to hurt you.\nYou lose 10 health\n");
        player.health -= 10;
        *combatEnded = 1;
    }
}
void healsquare1(){
    if(heal1 == 0){
    printf("You find a well cooked lahmacun and nice cold ayran\nYou eat them and feel refreshed:\n");
    Sleep(100);
    printf(" HP increased by 100.\n");
    player.health += 100;
    heal1 += 1;
    }
}
void healsquare2(){
    if(heal2 == 0){
        if(player.priestsstaff >= 1){
        printf("You find a shining apple, and take a bite thinking that it will give you super powers\n");
        Sleep(500);
        printf("Everything suddenly goes dark as you hear footsteps getting closer to you with a giggle.\nYou smell an alluring scent as you fall asleep\n");
        player.XLocation = 0;
        player.YLocation = 3;
        printf("\n\n");
        Sleep(3000);
        printf("You wake up naked in a bed in an unfamiliar place. Your vision is blurred.\nYou see a female figure leaving the place and dropping you an envelope...\n"
               "you stand up and feel dizzy due to the weird but beautiful scent in the room.\nYou take the envelope outside to open it and see what is inside of it.\n"
               "When you open the envelope you see lots of lipstick marks, heart and pentagram symbols on the paper inside\n"
               "You read the note inside the envelope:\n"
               "I marked your location on this map, head north from there if you wish to get your equipment back sweetie! <3 \n");
        heal2 += 1;
        turns += 4;
        player.succubusspell += 1;
        }
    }
}
void healsquare3(){
    if(heal3 == 0){
        printf("You find the PUBG Tournament winners and steal their chicken dinner\n");
        Sleep(1000);
        printf("You hide and enjoy their chicken dinner\n");
        Sleep(500);
        printf("HP increased by 45\t DEF increased by 1.\n");
        player.health += 45;
        player.defense += 1;
        heal3 += 1;
    }
}
void healsquare4(){
    if(heal4 == 0){
        printf("You step on a bear trap\nYour fast reflexes save you from a serious injury but you still take damage\n");
        Sleep(300);
        printf(" HP decreased by 80\n");
        player.health -= 80;
        heal4 += 1;
    }
}
void Attackvaluevsboss(int *combatEnded){
    int randomResult;
    time_t t;
    srand((unsigned) time(&t));
    randomResult = rand() % 100;
    if(randomResult > 100 - player.critchance){
        printf("Your turn: CRITICAL HIT\n");
        boss.bossHP -= player.attackdamage * player.critdamage - boss.bossDEF;
        *combatEnded = 0;
    }
    else if(randomResult < player.failchance){
        printf("Your turn: You try to hit, but DOOM POTATO parries and cut yourself.\n");
        player.health -= player.attackdamage;
        *combatEnded = 0;
    }
    else if(randomResult < 100 - player.critchance && randomResult > player.failchance){
        printf("Your turn: Clean hit!\n");
        boss.bossHP -= player.attackdamage - boss.bossDEF;
        *combatEnded = 0;
    }
    printf("\n");
}
void AttackvaluevsMiniboss(int *combatEnded){
    int randomResult;
    time_t t;
    srand((unsigned) time(&t));
    randomResult = rand() % 100;
    if(randomResult > 100 - player.critchance){
        printf("Your turn: CRITICAL HIT\n");
        miniboss.MinibossHP -= player.attackdamage * player.critdamage - miniboss.MinibossDEF;
        *combatEnded = 0;
    }
    else if(randomResult < player.failchance){
        printf("Your turn: Warlock dodged your attack and cut yourself.\n");
        player.health -= player.attackdamage;
        *combatEnded = 0;
    }
    else if(randomResult < 100 - player.critchance && randomResult > player.failchance){
        printf("Your turn: Clean hit!\n");
        miniboss.MinibossHP -= player.attackdamage - miniboss.MinibossDEF;
        *combatEnded = 0;
    }
    printf("\n");
}
void Attackvaluevssuccubus(int *combatEnded) {
    int randomResult;
    time_t t;
    srand((unsigned) time(&t));
    randomResult = rand() % 100;
    if (randomResult > 100 - player.critchance) {
        printf("Your turn: CRITICAL HIT\n");
        succubus.succubusHP -= player.attackdamage * player.critdamage - succubus.succubusDEF;
        *combatEnded = 0;
    } else if (randomResult < player.failchance) {
        printf("Your turn: Succubus dodges your attack.\n");
        //player.health -= player.attackdamage;
        *combatEnded = 0;
    } else if (randomResult < 100 - player.critchance && randomResult > player.failchance) {
        printf("Your turn: Clean hit!\n");
        succubus.succubusHP -= player.attackdamage - succubus.succubusDEF;
        *combatEnded = 0;
    }
    printf("\n");
}

void AttackvaluevsPriest(int *combatEnded) {
    int randomResult;
    time_t t;
    srand((unsigned) time(&t));
    randomResult = rand() % 100;
    if (randomResult > 100 - player.critchance) {
        printf("Your turn: CRITICAL HIT\n");
        priest.priestHP -= player.attackdamage * player.critdamage - priest.priestDEF;
        *combatEnded = 0;
    } else if (randomResult < 100 - player.critchance) {
        printf("Your turn: Clean hit!\n");
        priest.priestHP -= player.attackdamage - priest.priestDEF;
        *combatEnded = 0;
    }
    printf("\n");
}

void AttackvaluevsFoul(int *combatEnded) {
    int randomResult;
    time_t t;
    srand((unsigned) time(&t));
    randomResult = rand() % 100;
    if (randomResult > 100 - player.critchance) {
        printf("Your turn: CRITICAL HIT\n");
        foulspawn.foulHP -= player.attackdamage * player.critdamage;
        *combatEnded = 0;
    } else if (randomResult < 100 - player.critchance) {
        printf("Your turn: Clean hit!\n");
        foulspawn.foulHP -= player.attackdamage;
        *combatEnded = 0;
    }
    printf("\n");
}
void BossAttackvalue(int *combatEnded){
    int randomResult;
    time_t t;
    srand((unsigned) time(&t));
    randomResult = rand() % 100;
    if(randomResult > 100 - boss.bossCRT){
        printf("The DOOM POTATO's Turn: 'DOOM BASTARD!' CRITICAL HIT\n");
        player.health -= boss.bossATK * 2 - player.defense;
        *combatEnded = 0;
    }
    else if(randomResult < 100 - boss.bossCRT){
        printf("The DOOM POTATO's Turn: 'GRACE!' Clean hit!\n");
        player.health -= boss.bossATK - player.defense;
        *combatEnded = 0;
    }
    printf("Boss health %d\n", boss.bossHP);
    printf("Player health %d\n\n\n",player.health);
    printf("\n");
}
void MiniBossAttackvalue(int *combatEnded){
    int randomResult;
    time_t t;
    srand((unsigned) time(&t));
    randomResult = rand() % 100;
    if(randomResult > 100 - miniboss.MinibossCRT){
        printf("The Warlock counter attacks with a CRITICAL HIT\n");
        player.health -= miniboss.MinibossATK * 2 - player.defense;
        *combatEnded = 0;
    }
    else if(randomResult < 100 - miniboss.MinibossCRT){
        printf("The Warlock's Turn: Clean hit!\n");
        player.health -= miniboss.MinibossATK - player.defense;
        *combatEnded = 0;
    }
    //printf("The Warlock counter attacks\n");
    printf("Vile Warlock's health %d\n", miniboss.MinibossHP);
    printf("Player health %d\n\n\n",player.health);
    printf("\n");
}

void SuccubusAttackvalue(int *combatEnded){
    int randomResult;
    time_t t;
    srand((unsigned) time(&t));
    randomResult = rand() % 100;
    if(randomResult > 100 - succubus.succubusCRT){
        printf("The Succubus' Turn: She uses her black magic to deal magic damage!\n");
        player.health -= succubus.succubusATK * 2 - player.defense;
        *combatEnded = 0;
    }
    else if(randomResult < 100 - succubus.succubusCRT){
        printf("The Succubus' Turn: She lands a clean hit with her whip!\n");
        player.health -= succubus.succubusATK - player.defense;
        *combatEnded = 0;
    }
    //printf("The Succubus counter attacks\n");
    printf("Succubus health %d\n", succubus.succubusHP);
    printf("Player health %d\n\n\n",player.health);
    printf("\n");
}
void PriestAttackvalue(int *combatEnded){
    int randomResult;
    time_t t;
    srand((unsigned) time(&t));
    randomResult = rand() % 100;
    if(randomResult > 100 - priest.priestCRT){
        printf("The Priest's Turn: He uses his holy magic to deal magic damage!\n");
        player.health -= priest.priestATK * 2 - player.defense;
        *combatEnded = 0;
    }
    else if(randomResult < 100 - priest.priestCRT){
        printf("The Priest's Turn: He lands a clean hit with his staff!\n");
        player.health -= priest.priestATK - player.defense;
        *combatEnded = 0;
    }
    //printf("The Succubus counter attacks\n");
    printf("Priest's health %d\n", priest.priestHP);
    printf("Player health %d\n\n\n",player.health);
    printf("\n");
}

void FoulAttackvalue(int *combatEnded){
    int randomResult;
    time_t t;
    srand((unsigned) time(&t));
    randomResult = rand() % 100;
    if(randomResult > 100 - player.critchance){
        printf("That weird thing's Turn: It missed you?!\n");
        *combatEnded = 0;
    }
    else if(randomResult < 100 - player.critchance){
        printf("That weird thing's Turn: Clean hit!\n");
        player.health -= foulspawn.foulATK - player.defense;
        *combatEnded = 0;
    }
    printf("That thing's health %d\n", foulspawn.foulHP);
    printf("Player health %d\n\n\n",player.health);
    printf("\n");
}

void printmapandlocation(){
    for(int i = 0; i<5; i++){
        printf("***************************************************\n");
        for(int j = 0; j<3; j++){
            for(int k = 0; k < 6; k++){
                if(4-i == player.YLocation && k == player.XLocation)
                {
                    printf("* X  X  X ");
                }
                else{
                    printf("*         ");
                }
            }
            printf("\n");
        }
    }
    printf("***************************************************\n");
  //  turns -= 1;
}

void viewinventory(){

    printf("A shiny armor\nA rusty sword\nA cool pair of jeans\nA pair of trekking boots\n");
    if(player.potatopeeler == 1){
        printf("A HOLY POTATO PEELER\n");
    }
    if(player.priestsstaff == 1){
        printf("Staff of Wisdom\n");
    }
   // turns -=  1;
}

void printplayerstats(){
    printf("HP: %d\nATK: %d\nCRIT CHANCE: %d\nCRIT DAMAGE: x%d\nDEF: %d\nPOWER: Over 9000\n", player.health, player.attackdamage, player.critchance, player.critdamage, player.defense);
}
void savefile() {

    FILE *fp;

    fp = fopen("../Data/savedata.txt", "w+");
    if(fp == NULL)
    {
        printf("ERROR! FILE DOES NOT EXIST\n\a");
        exit(1);
    }
    fprintf(fp, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d" ,player.health, player.XLocation, player.YLocation, player.critchance, player.attackdamage, player.critdamage, player.failchance, player.defense, player.priestsstaff,player.priestsquest, player.potatopeeler, player.succubusspell, boss.bossHP, miniboss.MinibossHP, priest.priestHP, heal1, heal2, heal3, heal4, turns, healingRegen);
    fclose(fp);
    turns = turns - 1;

}
void savefile1() {

    FILE *fp;
    //char filename[20] = "savedata.txt";

    fp = fopen("../Data/savedata1.txt", "w+");
    if(fp == NULL)
    {
        printf("ERROR! FILE DOES NOT EXIST\n\a");
        exit(1);
    }

    fprintf(fp, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d" ,player.health, player.XLocation, player.YLocation, player.critchance, player.attackdamage, player.critdamage, player.failchance, player.defense, player.priestsstaff,player.priestsquest, player.potatopeeler, player.succubusspell, boss.bossHP, miniboss.MinibossHP, priest.priestHP, heal1, heal2, heal3, heal4, turns, healingRegen);
    fclose(fp);
    turns = turns - 1;
}
void savefile2() {

    FILE *fp;
    //char filename[20] = "savedata.txt";

    fp = fopen("../Data/savedata2.txt", "w+");
    if(fp == NULL)
    {
        printf("ERROR! FILE DOES NOT EXIST\n\a");
        exit(1);
    }

    fprintf(fp, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d" ,player.health, player.XLocation, player.YLocation, player.critchance, player.attackdamage, player.critdamage, player.failchance, player.defense, player.priestsstaff,player.priestsquest, player.potatopeeler, player.succubusspell, boss.bossHP, miniboss.MinibossHP, priest.priestHP, heal1, heal2, heal3, heal4, turns, healingRegen);
    fclose(fp);
    turns = turns - 1;
}

void loadfile() {

    FILE *fp;
    fp = fopen("../Data/savedata.txt", "r");
    if(fp == NULL)
    {
        printf("ERROR! FILE DOES NOT EXIST\n\a");
        exit(1);
    }
    fscanf(fp, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d" ,&player.health, &player.XLocation, &player.YLocation, &player.critchance, &player.attackdamage, &player.critdamage, &player.failchance, &player.defense, &player.priestsstaff,&player.priestsquest, &player.potatopeeler, &player.succubusspell, &boss.bossHP, &miniboss.MinibossHP, &priest.priestHP, &heal1, &heal2, &heal3, &heal4, &turns, &healingRegen);
    fclose(fp);
    turns = turns - 1;
}
void loadfile1() {

    FILE *fp;
    fp = fopen("../Data/savedata1.txt", "r");
    if(fp == NULL)
    {
        printf("ERROR! FILE DOES NOT EXIST\n\a");
        exit(1);
    }
    fscanf(fp, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d" ,&player.health, &player.XLocation, &player.YLocation, &player.critchance, &player.attackdamage, &player.critdamage, &player.failchance, &player.defense, &player.priestsstaff,&player.priestsquest, &player.potatopeeler, &player.succubusspell, &boss.bossHP, &miniboss.MinibossHP, &priest.priestHP, &heal1, &heal2, &heal3, &heal4, &turns, &healingRegen);
    fclose(fp);
    turns = turns - 1;
}
void loadfile2() {

    FILE *fp;
    fp = fopen("../Data/savedata2.txt", "r");
    if(fp == NULL)
    {
        printf("ERROR! FILE DOES NOT EXIST\n\a");
        exit(1);
    }
    fscanf(fp, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d" ,&player.health, &player.XLocation, &player.YLocation, &player.critchance, &player.attackdamage, &player.critdamage, &player.failchance, &player.defense, &player.priestsstaff,&player.priestsquest, &player.potatopeeler, &player.succubusspell, &boss.bossHP, &miniboss.MinibossHP, &priest.priestHP, &heal1, &heal2, &heal3, &heal4, &turns, &healingRegen);
    fclose(fp);
    turns = turns - 1;
}

void printsuccubus(){
    printf("_______ad88888888888888888888888a, \n"
           "________a88888'8888888888888888888888, \n"
           "______,8888'__'P88888888888888888888b, \n"
           "______d88_________`''P88888888888888888, \n"
           "_____,8888b_______________''88888888888888, \n"
           "_____d8P'''__,aa,______________''888888888b \n"
           "_____888bbdd888888ba,__,I_________'88888888, \n"
           "_____8888888888888888ba8'_________,88888888b \n"
           "____,888888888888888888b,________,8888888888 \n"
           "____(88888888888888888888,______,88888888888, \n"
           "____d888888888888888888888,____,8___'8888888b \n"
           "____88888888888888888888888__.;8''''__(888888 \n"
           "____8888888888888I'8888888P_,8'_,aaa,__888888 \n"
           "____888888888888I:8888888'_,8'__`b8d'__(88888 \n"
           "____(8888888888I'888888P'_,8)__________88888 \n"
           "_____88888888I'__8888P'__,8')__________88888 \n"
           "_____8888888I'___888'___,8'_(._.)_______88888 \n"
           "_____(8888I'_____'88,__,8'_____________,8888P \n"
           "______888I'_______'P8_,8'_____________,88888) \n"
           "_____(88I'__________',8'__M''''''M___,888888' \n"
           "____,8I'____________,8(____'aaaa'___,8888888 \n"
           "___,8I'____________,888a___________,8888888) \n"
           "__,8I'____________,888888,_______,888888888 \n"
           "_,8I'____________,8888888'`-===-'888888888' \n"
           ",8I'____________,8888888'________88888888'\n"
           "8I'____________,8'____88_________'888888P \n"
           "8I____________,8'_____88__________`P888'\n"
           "8I___________,8I______88____________'8ba,. \n"
           "(8,_________,8P'______88______________88''8bma,. \n"
           "_8I________,8P'_______88,______________'8b___''P8ma, \n"
           "_(8,______,8d'________`88,_______________'8b_____`'8a \n"
           "__8I_____,8dP_________,8X8,________________'8b.____:8b \n"
           "__(8____,8dP'__,I____,8XXX8,________________`88,____8) \n"
           "___8,___8dP'__,I____,8XxxxX8,_____I,_________8X8,__,8 \n"
           "___8I___8P'__,I____,8XxxxxxX8,_____I,________`8X88,I8 \n"
           "___I8,_'___,I____,8XxxxxxxxX8b,____I,________8XXX88I, \n"
           "___`8I______I'__,8XxxxxxxxxxxxXX8____I________8XXxxXX8, \n"
           "____8I_____(8__,8XxxxxxxxxxxxxxxX8___I________8XxxxxxXX8, \n"
           "___,8I_____I[_,8XxxxxxxxxxxxxxxxxX8__8________8XxxxxxxxX8, \n"
           "___d8I,____I[_8XxxxxxxxxxxxxxxxxxX8b_8_______(8XxxxxxxxxX8, \n"
           "___888I____`8,8XxxxxxxxxxxxxxxxxxxX8_8,_____,8XxxxxxxxxxxX8 \n"
           "___8888,____'88XxxxxxxxxxxxxxxxxxxX8)8I____.8XxxxxxxxxxxxX8 \n"
           "__,8888I_____88XxxxxxxxxxxxxxxxxxxX8_`8,__,8XxxxxxxxxxxxX8'\n"
           "__d88888_____`8XXxxxxxxxxxxxxxxxxX8'__`8,,8XxxxxxxxxxxxX8'\n"
           "__888888I_____`8XXxxxxxxxxxxxxxxX8'____'88XxxxxxxxxxxxX8'\n"
           "__88888888bbaaaa88XXxxxxxxxxxxXX8)______)8XXxxxxxxxxXX8'\n"
           "__8888888I,_``''''''8888888888888888aaaaa8888XxxxxXX8'\n"
           "__(8888888I,______________________.__```'''''88888P'\n"
           "___88888888I,___________________,8I___8,_______I8' \n"
           "____'''88888I,________________,8I'____'I8,____;8' \n"
           "___________`8I,_____________,8I'_______`I8,___8) \n"
           "____________`8I,___________,8I'__________I8__:8' \n"
           "_____________`8I,_________,8I'___________I8__:8 \n"
           "______________`8I_______,8I'_____________`8__(8 \n"
           "_______________8I_____,8I'________________8__(8; \n"
           "_______________8I____,8'__________________I___88, \n"
           "______________.8I___,8'_______________________8'8, \n"
           "______________(PI___'8_______________________,8,`8, \n"
           "_____________.88'____________,@@___________.a8X8,`8, \n"
           "_____________(88_____________@@@_________,a8XX888,`8, \n"
           "____________(888_____________@@'_______,d8XX8'__'b_`8, \n"
           "___________.8888,_____________________a8XXX8'____'a_`8, \n"
           "__________.888X88___________________,d8XX8I'______9,_`8, \n"
           "_________.88:8XX8,_________________a8XxX8I'_______`8__`8, \n"
           "________.88'_8XxX8a_____________,ad8XxX8I'________,8___`8, \n"
           "________d8'__8XxxxX8ba,______,ad8XxxX8I'__________8__,__`8, \n"
           "_______(8I___8XxxxxxX888888888XxxxX8I'____________8__II__`8 \n"
           "_______8I'___'8XxxxxxxxxxxxxxxxxxX8I'____________(8__8)___8; \n"
           "______(8I_____8XxxxxxxxxxxxxxxxxX8'______________(8__8)___8I \n"
           "______8P'_____(8XxxxxxxxxxxxxxX8I'________________8,_(8___:8 \n"
           "_____(8'_______8XxxxxxxxxxxxxxX8'_________________`8,_8____8 \n"
           "_____8I________`8XxxxxxxxxxxxX8'___________________`8,8___;8 \n"
           "_____8'_________`8XxxxxxxxxxX8'_____________________`8I__,8' \n"
           "_____8___________`8XxxxxxxxX8'_______________________8'_,8' \n"
           "_____8____________`8XxxxxxX8'________________________8_,8' \n"
           "_____8_____________`8XxxxX8'________________________d'_8' \n"
           "_____8______________`8XxxX8_________________________8_8' \n"
           "_____8________________'8X8'_________________________'8' \n"
           "_____8,________________`88___________________________8 \n"
           "_____8I________________,8'__________________________d) \n"
           "_____`8,_______________d8__________________________,8 \n"
           "______(b_______________8'_________________________,8' \n"
           "_______8,_____________dP_________________________,8' \n"
           "_______(b_____________8'________________________,8' \n"
           "________8,___________d8________________________,8' \n"
           "________(b___________8'_______________________,8' \n"
           "_________8,_________a8_______________________,8' \n"
           "_________(b_________8'______________________,8'\n"
           "__________8,_______,8______________________,8' \n"
           "__________(b_______8'_____________________,8' \n"
           "___________8,_____,8_____________________,8' \n"
           "___________(b_____8'____________________,8' \n"
           "____________8,___d8____________________,8' \n"
           "____________(b__,8'___________________,8' \n"
           "_____________8,,I8___________________,8' \n"
           "_____________I8I8'__________________,8' \n"
           "_____________`I8I__________________,8' \n"
           "______________I8'_________________,8' \n"
           "______________'8_________________,8' \n"
           "______________(8________________,8' \n"
           "______________8I_______________,8' \n"
           "______________(b,___8,________,8) \n"
           "______________`8I___'88______,8i8, \n"
           "_______________(b,__________,8'8') \n"
           "_______________`8I__,8______8)_8_8 \n"
           "________________8I__8I______'__8_8 \n"
           "________________(b__8I_________8_8 \n"
           "________________`8__(8,________b_8, \n"
           "_________________8___8)________'b'8, \n"
           "_________________8___8(_________'b'8 \n"
           "_________________8___'I__________'b8, \n"
           "_________________8________________`8) \n"
           "_________________8_________________I8 \n"
           "_________________8_________________(8 \n"
           "_________________8,_________________8, \n"
           "_________________Ib_________________8) \n"
           "_________________(8_________________I8 \n"
           "__________________8_________________I8 \n"
           "__________________8_________________I8 \n"
           "__________________8,________________I8 \n"
           "__________________Ib________________8I \n"
           "__________________(8_______________(8' \n"
           "___________________8_______________I8 \n"
           "___________________8,______________8I \n"
           "___________________Ib_____________(8' \n"
           "___________________(8_____________I8 \n"
           "___________________`8_____________8I \n"
           "____________________8____________(8' \n"
           "____________________8,___________I8 \n"
           "____________________Ib___________8I \n"
           "____________________(8___________8' \n"
           "_____________________8,_________(8 \n"
           "_____________________Ib_________I8 \n"
           "_____________________(8_________8I \n"
           "______________________8,________8' \n"
           "______________________(b_______(8 \n"
           "_______________________8,______I8 \n"
           "_______________________I8______I8 \n"
           "_______________________(8______I8 \n"
           "________________________8______I8, \n"
           "________________________8______8_8, \n"
           "________________________8,_____8_8' \n"
           "_______________________,I8_____'8' \n"
           "______________________,8'8,_____8, \n"
           "_____________________,8'_`8_____`b \n"
           "____________________,8'___8______8, \n"
           "___________________,8'____(a_____`b \n"
           "__________________,8'_____`8______8, \n"
           "__________________I8/______8______`b, \n"
           "__________________I8-/_____8_______`8, \n"
           "__________________(8/-/____8________`8, \n"
           "___________________8I/-/__,8_________`8 \n"
           "___________________`8I/--,I8________-8) \n"
           "____________________`8I,,d8I_______-8) \n"
           "______________________'bdI'8,_____-I8 \n"
           "___________________________`8,___-I8' \n"
           "____________________________`8,,--I8 \n"
           "_____________________________`Ib,,I8\n");
}

/*Definition for a room's available directions: Up, Right, Down, Left. 0 - blocked, 1 - available
{  //x=0
{ 1, 1, 0, 0}, //y=0
{ 1 ,1, 1, 0}, //y=1
{ 1, 1, 1, 0}, //y=2
{ 1, 1, 1, 0}, //y=3
{ 0, 1, 1, 0}, //y=4
},
{ //x=1
{ 1, 1, 0, 1}, //y=0
{ 1, 1, 1, 1}, //y=1
{ 1, 1, 1, 1}, //y=2
{ 1, 1, 1, 1}, //y=3
{ 0, 1, 1, 1}, //y=4
},
{ //x=2
{ 1, 1, 0, 1}, //y=0
{ 1, 1, 1, 1}, //y=1
{ 1, 1, 1, 1}, //y=2
{ 1, 1, 1, 1}, //y=3
{ 0, 1, 1, 1}, //y=4
},
{ //x=3
{ 1, 1, 0, 1}, //y=0
{ 1, 1, 1, 1}, //y=1
{ 1, 1, 1, 1}, //y=2
{ 1, 1, 1, 1}, //y=3
{ 0, 1, 1, 1}, //y=4
},
{ //x=4
{ 1, 0, 0, 1}, //y=0
{ 1, 0, 1, 1}, //y=1
{ 1, 0, 1, 1}, //y=2
{ 1, 0, 1, 1}, //y=3
{ 0, 0, 1, 1}, //y=4
}



void printinstructions(){
    printf("");
        for(int i = 0; i<5; i++){
        printf("*****************************************\n");
        for(int j = 0; j<3; j++){
            for(int k = 0; k < 6; k++){
                if(i == 1 && k == 2)
                {
                    printf("* X X X ");
                }
                else{
            printf("*       ");
                }
            }
            printf("\n");
        }
    }
    printf("*****************************************\n");
}


void printmap(){
for(int i = 0; i<3; i++){
    printf("****");
        for(int j = 0; j<3; j++){
            printf("*   *");
        }
}

 }





 1   2   3   4   5


 */