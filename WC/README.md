# WC  
WC je program, který na vstupu přijme cestu k souboru/ům a spoučítá četnosti jednotlivých slov obsažených uvnitř souborů. Program umožňuje, spočítat slova pro jednotlivé soubory (přepínač -s, --separate-files) a nebo pro všechny soubory najednou. Program funguje podobně jako linuxový příkaz "wc", tzn neumí přijímat aktivní vstup a běží jen 1x.

## Implementace
Program na vstupu přijme cestu k souboru. Otevře každý nalezený soubor a cyklicky prochází každý řádek a hledá mezery. V případě, že nalezne mezeru, ukončí načítání slova a odstraní za začátku a konce slova všechny nealfanumerické znaky. Slovo pak uloží do mapy jako klíč, případně zvýší počet nalezených slov pro daný klíč.

usage: WC [OPTION]...  [FILE]...  
options:  
-h, --help  to print help manual  
-s, --separate-files   count words for each file separated
