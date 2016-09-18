manga = {}
anime = {}
lightnovel = {}

try:
    datafile = open("MALdata.txt", "w+")
except:
    datafile = open("Maldata.txt", "w+")


userInput = input("Enter information in order: name, number, type\n")

userInput = userInput.split(",")
print(userInput)

if userInput[2].lower().strip() == "anime":
    anime[userInput[0].lower().strip()] = userInput[1].lower().strip()
    animeKeys = list(anime.keys())
    datafile.write("ANIME" + "-"*100 + "\n")
    datafile.write(animeKeys[0] + ": " + anime[animeKeys[0]] + "\n")
elif userInput[2].lower().strip() == "manga":
    manga[userInput[0].lower().strip()] = userInput[1].lower().strip()
    mangaKeys = list(manga.keys())
    datafile.write("MANGA" + "-" * 100 + "\n")
    datafile.write(mangaKeys[0] + ": " + manga[mangaKeys[0]] + "\n")
elif userInput[2].lower().strip() == "lightnovel":
    lightnovel[userInput[0].lower().strip()] = userInput[1].lower().strip()
    lightNovelKeys = list(lightnovel.keys())
    datafile.write("LIGHTNOVEL" + "-" * 100 + "\n")
    datafile.write(lightNovelKeys[0] + ": " + lightnovel[lightNovelKeys[0]] + "\n")
