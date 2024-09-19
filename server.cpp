#include <chrono>
#include <fstream>
#include <iostream>
#include <string>

#include <experimental/random>

constexpr auto MAX_WORDS = 3200000000;
constexpr auto MAX_MARIA = 16 * 1024 * 1024;

std::vector<std::string> book{"Tomek", "jest", "super", "fajny", "hacker", "i", "jest"};

std::string maria{"\
[Verse 1] \
The sun ablaze as Maria's foot touches the surface of sand \
On northern land, as human contraband \
Some rico from Jalisco passed her name to the Boss \
She stuffed into a truckbed, she clutches her cross \
Here comes the exhaust and it rips through her lungs \
She's off fast to the pasture, like cattle she'll cross \
Degree 106, sweat and vomit are thrown \
And she prays and suffocates upon the memories of home \
Of Yanqui guns for blood, debts on the loans \
Of smoldering fields, rape, rubble and bones \
Of graves hidden, trapped up in visions of war \
Of nothing, no one, nobody, no more \
[Chorus] \
These are her mountains and skies and she radiates \
Through history's rivers of blood, she regenerates \
And like the sun disappears only to reappear \
She's eternally here \
Her time is near \
Never conquered but here \
[Refrain] \
To tear away at the mask \
[Verse 2] \
And now she got a quota, the needle and thread crucifixion \
Sold and shipped across the new line of Mason Dixon \
Rippin' through denim, the point an inch from her vein \
The foreman approach, his steps now pound in her brain \
His presence, it terrifies and eclipses her days \
No minutes to rest, no moments to pray \
And with a whisper he whips her, her soul chained to his will \
\"My job is to kill if you forget to take your pill\" \
Her arms jerk, the sisters gather 'round her and scream \
As if in a dream, eyes on the crimson stream \
Numb as her wrists spit shots of blood to the floor \
I am nothing, no one, nobody, no more \
You might also like \
Guerrilla Radio \
Rage Against the Machine \
Mic Check \
Rage Against the Machine \
Voice of the Voiceless \
Rage Against the Machine \
[Chorus] \
These are her mountains and skies and she radiates \
Through history's rivers of blood, she regenerates \
And like the sun disappears only to reappear (Maria) \
She's eternally here \
Her time is near \
Never conquered but here \
[Refrain] \
To tear away at the mask \
Away at the mask \
To tear away at the mask \
[Bridge] \
No minutes to rest \
No moment to pray \
No minutes to rest \
No moment to pray \
No minutes to rest \
It eclipses her day \
Just a moment to pray \
[Refrain] \
To tear away at the mask \
Away at the mask \
To tear away at the mask \
To tear away at the mask \
"};

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " [filename] \n";

        return 0;
    }

    std::string filename = argv[1];
    std::ofstream ostrm(filename, std::ios::trunc | std::ios::binary);

    if (not ostrm.is_open())
        throw std::runtime_error("Stream hasn't an associated file");

    if (not ostrm.good())
        throw std::runtime_error("Error has occurred or I/O operations aren't available");

    const auto start = std::chrono::steady_clock::now();

    for (int i = 0; i < MAX_MARIA; i++)
    {
        std::cout << "\r" << (static_cast<double>(i) / static_cast<double>(MAX_MARIA)) * 100.0 << " %";

        ostrm << maria << " ";
    }

    /* for (size_t i = 0; i < MAX_WORDS; i++)
    {
        std::cout << "\r" << (static_cast<double>(i) / static_cast<double>(MAX_WORDS)) * 100.0 << " %";

        ostrm << book[std::experimental::randint(0, int(book.size() - 1))];

        // size_t len = std::experimental::randint(1, 16);

        // while (len-- > 0)
        // {
        //     ostrm << static_cast<char>(std::experimental::randint(int('a'), int('z')));
        // }

        ostrm << " ";
    } */

    const auto stop = std::chrono::steady_clock::now();
    const auto time = std::chrono::duration_cast<std::chrono::seconds>(stop - start).count();

    std::cout << "\n" << "time: " << time << " s\n";

    return 0;
}
