#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// global variables
#define KILLER_COUNT 41
#define PERK_KILLER_COUNT 136
#define KILLER_ADDON_COUNT 20
#define SURVIVOR_COUNT 49
#define PERK_SURVIVOR_COUNT 161
#define FLASHLIGHT_COUNT 3
#define TOOLBOX_COUNT 6
#define MEDKIT_COUNT 4
#define FOGVIAL_COUNT 3
#define MAP_COUNT 3
#define KEY_COUNT 3
#define FLASHLIGHT_ADDON_COUNT 13
#define TOOLBOX_ADDON_COUNT 11
#define MEDKIT_ADDON_COUNT 13
#define FOGVIAL_ADDON_COUNT 5
#define MAP_ADDON_COUNT 5
#define KEY_ADDON_COUNT 5
#define SURVIVOR_ITEM_COUNT 6
#define KILLER_OFFERING_COUNT 39
#define SURVIVOR_OFFERING_COUNT 43
// history txt
#define HISTORY_FILE "DBD 9.3.0. History.txt"

// ANSI color codes
#define RESET             "\x1b[0m" // resets the color to white
#define COLOR_COMMON      "\x1b[38;5;94m" // brown
#define COLOR_UNCOMMON    "\x1b[32m" // green
#define COLOR_RARE        "\x1b[34m" // blue
#define COLOR_VERY_RARE   "\x1b[35m" // magenta close enough to purple
#define COLOR_VISCERAL    "\x1b[31m" // red
#define COLOR_KILLER      "\x1b[1;31m" // bold red
#define COLOR_SURVIVOR    "\e[1;33m" // bold yellow
#define COLOR_PERK        "\x1b[36m" // cyan

// ANSI string compares to the rarity list, then it returns a global color variable when called
const char* rarityColor(const char* rarity) {
    if      (strcmp(rarity, "Common") == 0) 
        return COLOR_COMMON;
    else if (strcmp(rarity, "Uncommon") == 0) 
        return COLOR_UNCOMMON;
    else if (strcmp(rarity, "Rare") == 0) 
        return COLOR_RARE;
    else if (strcmp(rarity, "Very Rare") == 0) 
        return COLOR_VERY_RARE;
    else if (strcmp(rarity, "Visceral") == 0) 
        return COLOR_VISCERAL;
    else 
        return RESET;
}

int main () {
    // Meant for dbd version 9.2.0.
    // really bad and unorganized int declaration
    int option, randKiller, randKillerPerks, randKillerAddon, randKillerOffering, killer, survivor, randSurvivor, randSurvivorPerks, randSurvivorItem, randSurvivorAddon, randSurvivorOffering, perk1, perk2, perk3, perk4, item, subItem, addon1, addon2;
    // having to give these values for the ID system 
    int a1 = 0, a2 = 0;
    int selected[4] = {0,0,0,0};
    int count = 0;
    int offering = 0;
    //giant giant survivor list
    char *survivorList[SURVIVOR_COUNT] = {
        "Dwight Fairfield", "Meg Thomas", "Claudette Morel", "Jake Park",
        "Nea Karlsson", "Bill Overbeck", "Laurie Strode", "Ace Visconti",
        "Feng Min", "David King", "Quentin Smith", "David Tapp",
        "Kate Denson", "Adam Francis", "Jeff Johansen", "Jane Romero",
        "Ash Williams", "Nancy Wheeler", "Steve Harrington", "Yui Kimura",
        "Zarina Kassir", "Cheryl Mason", "Felix Richter", "Élodie Rakoto",
        "Yun-Jin Lee", "Jill Valentine", "Leon Scott Kennedy", "Mikaela Reid",
        "Jonah Vasquez", "Yoichi Asakawa", "Haddie Kaur", "Ada Wong",
        "Rebecca Chambers", "Vittorio Toscano", "Thalita Lyra", "Renato Lyra",
        "Gabriel Soma", "Nicolas Cage", "Ellen Ripley", "Alan Wake",
        "Sable Ward", "Aestri Yazar & Baermar Uraz", "Lara Croft", "Trevor Belmont",
        "Taurie Cain", "Orela Rose", "Rick Grimes", "Michonne Grimes", 
        "Vee Boonyasak"
    };
    // survivor perk list in alphabetical order as shown ingame
    char *perkSurvivorList[PERK_SURVIVOR_COUNT] = {
        //page 1
        "Ace In The Hole", "Adrenaline", "Aftercare", "Alert", "Any Means Necessary",
        "Apocalyptic Ingenuity", "Appraisal", "Autodidact", "Babysitter", "Background Player",
        "Balanced Landing", "Bardic Inspiration", "Better Than New", "Better Together", "Bite The Bullet",
        //page 2
        "Blast Mine", "Blood Pact", "Blood Rush", "Boil Over", "Bond",
        "Boon: Circle Of Healing", "Boon: Dark Theory", "Boon: Exponential", "Boon: Illumination", "Boon: Shadow Step",
        "Borrowed Time", "Botany Knowledge", "Breakdown", "Breakout", "Buckle Up",
        //page 3
        "Built To Last", "Calm Spirit", "Camaraderie", "Champion Of Light", "Chemical Trap",
        "Clairvoyance", "Clean Break", "Come And Get Me!", "Convinction", "Corrective Action",
        "Counterforce", "Cut Loose", "Dance With Me", "Dark Sense", "Dead Hard",
        //page 4
        "Deadline", "Deception", "Decisive Strike", "Deja Vu", "Deliverance",
        "Desperate Measures", "Detective's Hunch", "Distortion", "Diversion", "Do No Harm",
        "Dramaturgy", "Duty Of Care", "Empathic Connection", "Empathy", "Exultation",
        //page 5
        "Eyes Of Belmont", "Fast Track", "Finesse", "Fixated", "Flashbang",
        "Flip-Flop", "Fogwise", "For The People", "Friendly Competition", "Ghost Notes", 
        "Hardened", "Head On", "Hope", "Hyperfocus", "Inner Focus", 
        //page 6
        "Inner Strength", "Invocation: Treacherous Crows", "Invocation: Weaving Spiders", "Iron Will", "Kindred", 
        "Last Stand", "Leader", "Left Behind", "Light-Footed", "Lightweight", 
        "Lithe", "Low Profile", "Lucky Break", "Lucky Star", "Made For This", 
        //page 7
        "Mettle Of Man", "Mirrored Illusion", "Moment Of Glory", "No Mither", "No One Left Behind", 
        "Object Of Obsession", "Off The Record", "One-Two-Three-Four!", "Open-Handed", "Overcome", 
        "Overzealous", "Parental Guidance", "Pharmacy", "Plot Twist", "Plunderer's Instinct", 
        //page 8
        "Poised", "Potential Energy", "Power Struggle", "Premonition", "Prove Thyself", 
        "Quick & Quiet", "Quick Gambit", "Rapid Response", "Reactive Healing", "Reassurance", 
        "Red Herring", "Repressed Alliance", "Residual Manifest", "Resilience", "Resurgence", 
        //page 9
        "Road Life", "Rookie Spirit", "Saboteur", "Scavenger", "Scene Partner", 
        "Second Wind", "Self-Care", "Self-Preservation", "Shoulder The Burden", "Slippery Meat", 
        "Small Game", "Smash Hit", "Sole Survivor", "Solidarity", "Soul Guard", 
        //page 10
        "Specialist", "Spine Chill", "Sprint Burst", "Stake Out", "Still Sight", 
        "Streetwise", "Strength In Shadows", "Teamwork: Collective Stealth", "Teamwork: Power Of Two", "Teamwork: Throw Down",
        "Teamwork: Toughen Up", "Technician", "Tenacity", "This Is Not Happening", "Troubleshooter", 
        //page 11
        "Unbreakable", "Up The Ante", "Urban Evasion", "Vigil", "Visionary", 
        "Wake Up!", "We'll Make It", "We're Gonna Live Forever", "Wicked", "Windows Of Opportunity", 
        "Wiretap"
    };
    //all flashlights and addons with respect to its corresponding rarities
    char *flashlight[FLASHLIGHT_COUNT] = {
        "Utility Flashlight", "Sport Flashlight", "Flashlight"
    };
    char *flashlightRarity[FLASHLIGHT_COUNT] = {
        "Very Rare", "Rare", "Uncommon"
    };
    char *flashlightAddon[FLASHLIGHT_ADDON_COUNT] = {
        "Odd Bulb", "High-End Sapphire Lens", "Intense Halogen", "Long Life Battery", "Focus Lens",
        "Heavy Duty Battery", "Low Amp Filament", "Rubber Grip", "Tir Optic", "Battery",
        "Leather Grip", "Power Bulb", "Wide Lens"
    };
    char *flashlightAddonRarity[FLASHLIGHT_ADDON_COUNT] = {
        "Visceral", "Very Rare", "Rare", "Rare", "Uncommon",
        "Uncommon", "Uncommon", "Uncommon", "Uncommon", "Common",
        "Common", "Common", "Common"
    };
    //all medkits and addons with respect to its corresponding rarities
    char *medkit[MEDKIT_COUNT] = {
        "Ranger Med-Kit", "Emergency Med-Kit", "First Aid Kit", "Camping Aid Kit"
    };
    char *medkitRarity[MEDKIT_COUNT] = {
        "Very Rare", "Rare", "Uncommon", "Common"
    };
    char *medkitAddon[MEDKIT_ADDON_COUNT] = {
        "Anti-Hemorrhagic Syringe", "Gel Dressings", "Abdominal Dressing", "Styptic Agent", "Gauze Roll",
        "Surgical Suture", "Medical Scissors", "Needle & Thread", "Self Adherent Wrap", "Sponge",
        "Bandages", "Butterfly Tape", "Rubber Gloves"
    };
    char *medkitAddonRarity[MEDKIT_ADDON_COUNT] = {
        "Visceral", "Visceral", "Very Rare", "Very Rare", "Rare",
        "Rare", "Uncommon", "Uncommon", "Uncommon", "Uncommon",
        "Common", "Common", "Common"
    };
    //all toolboxes and addons with respect to its corresponding rarities
    char *toolbox[TOOLBOX_COUNT] = {
        "Alex's Toolbox", "Engineer's Toolbox", "Commodious Toolbox", "Mechanic's Toolbox", "Toolbox", "Worn-Out Tools"
    };
    char *toolboxRarity[TOOLBOX_COUNT] = {
        "Very Rare", "Very Rare", "Rare", "Rare", "Uncommon", "Common"
    };
    char *toolboxAddon[TOOLBOX_ADDON_COUNT] = {
        "Brand New Part", "Grip Wrench", "Hacksaw", "Cutting Wire", "Protective Gloves",
        "Socket Swivels", "Spring Clamp", "Wire Spool", "Clean Rag", "Instructions",
        "Scraps"
    };
    char *toolboxAddonRarity[TOOLBOX_ADDON_COUNT] = {
        "Visceral", "Rare", "Rare", "Uncommon", "Uncommon",
        "Uncommon", "Uncommon", "Uncommon", "Common", "Common",
        "Common"
    };
    //all fog vials and addons with respect to its corresponding rarities
    char *fogVial[FOGVIAL_COUNT] = {
        "Vigo's Fog Vial", "Artisan's Fog Vial", "Apprentice's Fog Vial"
    };
    char *fogVialRarity[FOGVIAL_COUNT] = {
        "Rare", "Uncommon", "Common"
    };
    char *fogVialAddon[FOGVIAL_ADDON_COUNT] = {
        "Potent Extract", "Mushroom Formula", "Oily Sap", "Reactive Compound", "Volcanic Stone"
    };
    char *fogVialAddonRarity[FOGVIAL_ADDON_COUNT] = {
        "Visceral", "Very Rare", "Rare", "Uncommon", "Common"
    };
    //all maps and addons with respect to its corresponding rarities
    char *map[MAP_COUNT] = {
        "Annotated Map", "Scribbled Map", "Cryptic Map"
    };
    char *mapRarity[MAP_COUNT] = {
        "Rare", "Uncommon", "Common"
    };
    char *mapAddon[MAP_ADDON_COUNT] = {
        "Crimson Stamp", "Sharpened Flint", "Battered Tape", "Gnarled Compass", "Glowing Ink"
    };
    char *mapAddonRarity[MAP_ADDON_COUNT] = {
        "Visceral", "Very Rare", "Rare", "Uncommon", "Common"
    };
    //all keys and addons with respect to its corresponding rarities
    char *key[KEY_COUNT] = {
        "Skeleton Key", "Dull Key", "Broken Key"
    };
    char *keyRarity[KEY_COUNT] = {
        "Rare", "Uncommon", "Common"
    };
    char *keyAddon[KEY_ADDON_COUNT] = {
        "Blood Amber", "Unique Wedding Ring", "Braided Bauble", "Shrill Whistle", "Friendship Charm"
    };
    char *keyAddonRarity[KEY_ADDON_COUNT] = {
        "Visceral", "Very Rare", "Rare", "Uncommon", "Common"
    };
    //addon item list to know which addon it is calling on
    char **survivorItemAddons[SURVIVOR_ITEM_COUNT] = {
        flashlightAddon,
        medkitAddon,
        toolboxAddon,
        fogVialAddon,
        mapAddon,
        keyAddon
    };
    //addon rarity list to know what rarity each addon is
    char **survivorItemAddonRarity[SURVIVOR_ITEM_COUNT] = {
        flashlightAddonRarity,
        medkitAddonRarity,
        toolboxAddonRarity,
        fogVialAddonRarity,
        mapAddonRarity,
        keyAddonRarity
    };

    // number of addons for each survivor item type
    int survivorItemAddonCount[SURVIVOR_ITEM_COUNT] = {
        FLASHLIGHT_ADDON_COUNT,
        MEDKIT_ADDON_COUNT,
        TOOLBOX_ADDON_COUNT,
        FOGVIAL_ADDON_COUNT,
        MAP_ADDON_COUNT,
        KEY_ADDON_COUNT
    };

    // number of items for each survivor item type
    int survivorItemCount[SURVIVOR_ITEM_COUNT] = {
        FLASHLIGHT_COUNT,
        MEDKIT_COUNT,
        TOOLBOX_COUNT,
        FOGVIAL_COUNT,
        MAP_COUNT,
        KEY_COUNT
    };
    //called when doing rand for survivor item
    char **survivorItem[SURVIVOR_ITEM_COUNT] = {
        flashlight,
        medkit,
        toolbox,
        fogVial,
        map,
        key,
    };

    //item rarity list from most valuable to most common
    char **survivorItemRarity[SURVIVOR_ITEM_COUNT] = {
        flashlightRarity,
        medkitRarity,       
        toolboxRarity,      
        fogVialRarity,      
        mapRarity,         
        keyRarity           
    };

    //giant killer list
    char *killerList[KILLER_COUNT] = {
        "The Trapper", "The Wraith", "The Hillbilly", "The Nurse", 
        "The Shape", "The Hag", "The Doctor", "The Cannibal",
        "The Huntress", "The Nightmare", "The Pig", "The Clown", 
        "The Spirit", "The Legion", "The Plague", "The Ghost Face", 
        "The Demogorgon", "The Oni", "The Deathslinger", "The Executioner",
        "The Blight", "The Twins", "The Trickster", "The Nemesis", 
        "The Cenobite", "The Artist", "The Onryo", "The Dredge", 
        "The Mastermind", "The Knight", "The Skull Merchant", "The Singularity",
        "The Xenomorph", "The Good Guy", "The Unknown", "The Lich",
        "The Dark Lord", "The HoundMaster", "The Ghoul", "The Animatronic",
        "The Krasue"
    };
    //all the killer perks as shown ingame
    char *perkKillerList[PERK_KILLER_COUNT] = {
        //page 1
        "A Nurse's Calling", "Agitation", "Alien Instinct", "All-Shaking Thunder", "Awakened Awareness",
        "Bamboozle", "Barbecue & Chili", "Batteries Included", "Beast Of Prey", "Bitter Murmur",
        "Blood Echo", "Blood Warden", "Bloodhound", "Brutal Strength", "Call Of Brine",
        //page 2
        "Corrupt Intervention", "Coulrophobia", "Coup De Grâce", "Cruel Limits", "Dark Arrogance",
        "Dark Devotion", "Darkness Revealed", "Dead Man's Switch", "Deadlock", "Deathbound",
        "Deerstalker", "Discordance", "Dissolution", "Distressing", "Dominance",
        //page 3
        "Dragon's Grip", "Dying Light", "Enduring", "Eruption", "Fire Up",
        "Forced Hesitation", "Forced Penance", "Forever Entwined", "Franklin's Demise", "Friends 'Til The End",
        "Furtive Chase", "Game Afoot", "Gearhead", "Genetic Limits", "Grim Embrace",
        //page 4
        "Haywire", "Help Wanted", "Hex: Blood Favor", "Hex: Crowd Control", "Hex: Devour Hope",
        "Hex: Face The Darkness", "Hex: Haunted Ground", "Hex: Huntress Lullaby", "Hex: No One Escapes Death", "Hex: Nothing But Misery",
        "Hex: Overture of Doom", "Hex: Pentimento", "Hex: Plaything", "Hex: Retribution", "Hex: Ruin", 
        //page 5
        "Hex: The Third Seal", "Hex: Thrill Of The Hunt", "Hex: Two Can Play", "Hex: Undying", "Hex: Wretched Fate", 
        "Hoarder", "Hubris", "Human Greed", "Hysteria", "I'm All Ears", 
        "Infectious Fright", "Insidious", "Iron Grasp", "Iron Maiden", "Knock Out",
        //page 6
        "Languid Touch", "Lethal Pursuer", "Leverage", "Lightborn", "Machine Learning", 
        "Mad Grit", "Make Your Choice", "Merciless Storm", "Mindbreaker", "Monitor & Abuse", 
        "Nemesis", "No Quarter", "No Way Out", "None Are Free", "Nowhere To Hide", 
        //page 7
        "Oppression", "Overcharge", "Overwhelming Presence", "Phantom Fear", "Play With Your Food", 
        "Pop Goes The Weasel", "Predator", "Rancor", "Rapid Brutality", "Ravenous", 
        "Remember Me", "Save The Best For Last", "Scourge Hook: Floods Of Rage", "Scourge Hook: Gift Of Pain", "Scourge Hook: Hangman's Trick", 
        //page 8
        "Scourge Hook: Jagged Compass", "Scourge Hook: Monstrous Shrine", "Scourge Hook: Pain Resonance", "Septic Touch", "Shadowborn", 
        "Shattered Hope", "Sloppy Butcher", "Spies From The Shadows", "Spirit Fury", "Starstruck", 
        "Stridor", "Superior Anatomy", "Surge", "Surveillance", "Terminus", 
        //page 9
        "Territorial Imperative", "Thanatophobia", "Thrilling Tremors", "Thwack!", "Tinkerer", 
        "Trail of Torment", "Ultimate Weapon", "Unbound", "Undone", "Unforeseen", 
        "Unnerving Presence", "Unrelenting", "Wandering Eye", "Weave Attunement", "Whispers", 
        //page 10
        "Zanshin Tactics" 
    };
    //yeah all the dumb killer addons, never touching this crap ever again
    char *trapperAddons[KILLER_ADDON_COUNT] = {
        "Bloody Coil", "Iridescent Stone", "Honing Stone", "Oily Coil", "Tension Spring", 
        "Trapper Sack", "Fastening Tools", "Rusted Jaws", "Secondary Coil", "Tar Bottle", 
        "Trapper Bag", "4-Coil Spring Kit", "Coffee Grounds", "Lengthened Jaws", "Serrated Jaws", 
        "Wax Brick", "Bear Oil", "Makeshift Wrap", "Padded Jaws", "Trapper Gloves"
    };
    char *wraithAddons[KILLER_ADDON_COUNT] = {
        "\"All Seeing\" - Spirit", "Coxcombed Clapper", "\"All Seeing\" - Blood", "\"Shadow Dance\" - Blood", "\"Swift Hunt\" - Blood",
        "\"Windstorm\" - Blood", "\"Blind Warrior\" - White", "\"Blink\" - White", "\"Shadow Dance\" - White", "\"Swift Hunt\" - White",
        "\"Windstorm\" - White", "\"Blind Warrior\" - Mud", "\"Blink\" - Mud", "\"Swift Hunt\" - Mud", "\"Windstorm\" - Mud",
        "Bone Clapper", "\"The Beast\" - Soot", "\"The Ghost\" - Soot", "\"The Hound\" - Soot", "\"The Serpent\" - Soot"
    };
    char *hillbillyAddons[KILLER_ADDON_COUNT] = {
        "Iridescent Engravings", "Tuned Carburetor", "Apex Muffler", "Filthy Slippers", "Lo Pro Chains",
        "Spiked Boots", "Begrimed Chains", "Dad's Boots", "Low Kickback Chains", "Ragged Engine",
        "The Thompsons' Mix", "Clogged Intake", "Greased Throttle", "High-Speed Idler Screw", "Off-Brand Motor Oil",
        "Thermal Casing", "Counterweight", "Cracked Primer Bulb", "Discarded Air Filter", "Steel Toe Boots"
    };
    char *nurseAddons[KILLER_ADDON_COUNT] = {
        "Matchbox", "Torn Bookmark", "\"Bad Man's\" Last Breath", "Campbell's Last Breath", "Jenner's Last Breath",
        "Kavanagh's Last Breath", "Anxious Gasp", "Ataxic Respiration", "Fragile Wheeze", "Heavy Panting",
        "Spasmodic Breath", "Bad Man Keepsake", "Cataonic Boy's Treasure", "Dark Cincture", "Dull Bracelet",
        "Pocket Watch", "Metal Spoon", "Plaid Flannel", "White Nit Comb", "Wooden Horse"
    };
    char *shapeAddons[KILLER_ADDON_COUNT] = {
        "Judith's Tombstone", "Scratched Mirror", "Lock Of Hair", "Reflective Fragment", "Tombstone Piece",
        "Vanity Mirror", "Hair Bow", "J.Myers Memorial", "Jewelry Box", "Judith's Journal",
        "Mirror Shard", "Dead Rabbit", "Fragrant Tuft Of Hair", "Glass Fragment", "Hair Brush",
        "Jewelry", "Blond Hair", "Boyfriend's Memo", "Memorial Flower", "Tacky Earrings"
    };
    char *hagAddons[KILLER_ADDON_COUNT] = {
        "Mint Rag", "Waterlogged Shoe", "Disfigured Ear", "Granma's Heart", "Rusty Shackles",
        "Scarred Hand", "Bloodied Mud", "Cracked Turtle Egg", "Dried Cicada", "Swamp Orchid Necklet",
        "Willow Wreath", "Bloodied Water", "Cypress Necklet", "Dragonfly Wings", "Half Eggshell",
        "Pussy Willow Catkins", "Bog Water", "Dead Fly Mud", "Powdered Eggshell", "Rope Necklet"
    };
    char *doctorAddons[KILLER_ADDON_COUNT] = {
        "Iridescent King", "Iridescent Queen", "\"Calm\" - Carter's Notes", "\"Discipline\" - Carter's Notes", "\"Order\" - Carter's Notes",
        "\"Restraint\" - Carter's Notes", "\"Discipline\" - Class III", "\"Restraint\" - Class III", "High Stimulus Electrode", "Interview Tape",
        "Scrapped Tape", "\"Calm\" - Class II", "\"Discipline\" - Class II", "\"Order\" - Class II", "\"Restraint\" - Class II",
        "Polished Electrode", "\"Calm\" - Class I", "\"Order\" - Class I", "Maple Knight", "Moldy Electrode"
    };
    char *cannibalAddons[KILLER_ADDON_COUNT] = {
        "Carburetor Tuning Guide", "Iridescent Flesh", "Award-Winning Chili", "Depth Gauge Rake", "Light Chassis",
        "Rusted Chain", "Begrimed Chains", "Grisly Chain", "Shop Lubricant", "The Beast's Marks",
        "The Grease", "Chili", "Homemade Muffler", "Knife Scratches", "Long Guide Bar", 
        "Primer Bulb", "Chainsaw File", "Spark Plug", "Speed Limiter", "Vegetable Oil"
    };
    char *huntressAddons[KILLER_ADDON_COUNT] = {
        "Iridescent Head", "Soldier's Puttee", "Begrimed Head", "Glowing Concoction", "Infantry Belt",
        "Wooden Fox", "Deerskin Gloves", "Flower Babushka", "Rose Root", "Rusty Head",
        "Venomous Concoction", "Leather Loop", "Manna Grass Braid", "Oak Haft", "Shiny Pin",
        "Weighted Head", "Amanita Toxin", "Bandaged Haft", "Coarse Stone", "Yellowed Cloth"
    };
    char *nightmareAddons[KILLER_ADDON_COUNT] = {
        "Black Box", "Red Paint Brush", "\"Z\" Block", "Class Photo", "Pill Bottle",
        "Swing Chains", "Blue Dress", "Jump Rope", "Nancy's Masterpiece", "Paint Thinner",
        "Unicorn Block", "Cat Block", "Green Dress", "Nancy's Sketch", "Outdoor Rope",
        "Prototype Claws", "Garden Rake", "Kid's Drawing", "Sheep Block", "Wool Shirt"
    };
    char *pigAddons[KILLER_ADDON_COUNT] = {
        "Amanda's Letter", "Video Tape", "Amanda's Secret", "Crate Of Gears", "Jigsaw's Sketch",
        "Tampered Timer", "Bag Of Gears", "Jigsaw's Annotated Plan", "Rule Set No.2", "Rusty Attachments",
        "Slow-Release Toxin", "Face Mask", "Last Will", "Razor Wires", "Utility Blades",
        "Workshop Grease", "Combat Straps", "Interlocking Razor", "John's Medical File", "Shattered Syringe"
    };
    char *clownAddons[KILLER_ADDON_COUNT] = {
        "Redhead's Pinky Finger", "Tattoo's Middle Finger", "Cheap Gin Bottle", "Cigar Box", "Ether 15 Vol%%",
        "Garnish Makeup Kit", "Bottle Of Chloroform", "Flask Of Bleach", "Smelly Inner Soles", "Spirit Of Hartshorn",
        "Sulfuric Acid Vial", "Kerosene Can", "Solvent Jug", "Starling Feather", "Sticky Soda Bottle",
        "Thick Cork Stopper", "Fingerless Parade Gloves", "Party Bottle", "Robin Feather", "VHS Porn"
    };
    char *spiritAddons[KILLER_ADDON_COUNT] = {
        "Kintsugi Teapcup", "Mother-Daughter Ring", "Dried Cherry Blossom", "Furin", "Wakizashi Saya",
        "Yakuyoke Amulet", "Katana Tsuba", "Mother's Glasses", "Rusty Flute", "Senko Hanabi",
        "Uchiwa", "Juniper Bonsai", "Kaiun Talisman", "Muddy Sports Day Cap", "Rin's Broken Watch",
        "White Hair Ribbon", "Gifted Bamboo Comb", "Origami Crane", "Shiawase Amulet", "Zōri"
    };
    char *legionAddons[KILLER_ADDON_COUNT] = {
        "Fuming Mix Tape", "Iridescent Button", "BFFs", "Filthy Blade", "Frank's Mix Tape",
        "Stab Wounds Study", "Joey's Mix Tape", "Stolen Sketch Book", "Stylish Sunglasses", "Susie's Mix Tape",
        "The Legion Pin", "Defaced Smiley Pin", "Etched Ruler", "Julie's Mix Tape", "Mural Sketch",
        "Never-Sleep Pills", "Friendship Bracelet", "Mischief List", "Scratched Ruler", "Smiley Face Pin"
    };
    char *plagueAddons[KILLER_ADDON_COUNT] = {
        "Black Incense", "Iridescent Seal", "Devotee's Amulet", "Severed Toe", "Vile Emetic",
        "Worship Tablet", "Ashen Apple", "Exorcism Amulet", "Incensed Ointment", "Infected Emetic",
        "Rubbing Oil", "Blessed Apple", "Emetic Potion", "Hematite Seal", "Potent Tincture",
        "Prophylactic Amulet", "Healing Salve", "Limestone Seal", "Olibanum Incense", "Prayer Tablet Fragment"
    };
    char *ghostfaceAddons[KILLER_ADDON_COUNT] = {
        "Outdoor Security Camera", "\"Ghost Face Caught On Tape\"", "Driver's License", "Drop-Leg Knife Sheath", "Night Vision Monocular",
        "Victim's Deatiled Routine", "Chewed Pen", "Knight Belt Clip", "Lasting Perfume", "Leather Knife Sheath",
        "Olsen's Wallet", "Cinch Straps", "Marked Map", "Olsen's Address Book", "Olsen's Journal",
        "Telephoto Lens", "Cheap Cologne", "Headline Cutouts", "Walleye's Matchbook", "\"Philly\""
    };
    char *demogorgonAddons[KILLER_ADDON_COUNT] = {
        "Leprose Lichen", "Red Moss", "Lifeguard Whistle", "Unknown Egg", "Upside Down Resin",
        "Vermilion Webcap", "Brass Case Lighter", "Deer Lung", "Eleven's Soda", "Thorny Vines",
        "Violet Waxcap", "Barb's Glasses", "Mew's Guts", "Rotten Green Tripe", "Sticky Lining",
        "Viscou Webbing", "Black Heart", "Rat Liver", "Rat Tail", "Rotten Pumpkin"
    };
    char *oniAddons[KILLER_ADDON_COUNT] = {
        "Iridescent Family Crest", "Renjiro's Bloody Glove", "Akito's Crutch", "Lion Fang", "Splintered Hull",
        "Tear Soaked Tenugui", "Kanai-Anzen Talisman", "Scalped Topknot", "Shattered Wakizashi", "Wooden Oni Mask",
        "Yamaoka Sashimono", "Bloody Sash", "Child's Wooden Sword", "Chipped Saihai", "Ink Lion",
        "Polished Maedate", "Blackened Toenail", "Cracked Sakazuki", "Paper Lantern", "Rotting Rope"
    };
    char *deathslingerAddons[KILLER_ADDON_COUNT] = {
        "Hellshire Iron", "Iridescent Coin", "Barbed Wire", "Bayshore's Cigar", "Gold Creek Whiskey",
        "Prison Chain", "Bayshore's Gold Tooth", "Honey Locust Thorns", "Tin Oil Can", "Wanted Poster",
        "Warden's Keys", "Chewing Tobacco", "Jaw Smasher", "Marshal's Badge", "Poison Oak Leaves",
        "Rusted Spike", "Modified Ammo Belt", "Rickety Chain", "Snake Oil", "Spit Polish Rag"
    };
    char *executionerAddons[KILLER_ADDON_COUNT] = {
        "Iridescent Seal Of Metatron", "Obsidian Goblet", "Crimson Ceremony Book", "Lost Memories Book", "Rust-Colored Egg",
        "Scarlet Egg", "Burning Man Painting", "Mannequin Foot", "Misty Day, Remains Of Judgement", "Tablet Of The Oppressor",
        "Valtiel Sect Photograph", "Cinderella Music Box", "Forgotten Videotape", "Leopard-Print Fabric", "Spearhead",
        "Wax Doll", "Black Strap", "Copper Ring", "Dead Butterfly", "Lead Ring"
    };
    char *blightAddons[KILLER_ADDON_COUNT] = {
        "Compound Thirty-Three", "Iridescent Blight Tag", "Alchemist's Ring", "Soul Chemical", "Summoning Stone",
        "Vigo's Journal", "Adrenaline Vial", "Blighted Crow", "Compound Twenty-One", "Rose Tonic",
        "Umbra Salts", "Blighted Rat", "Canker Thorn", "Plague Bile", "Pustula Dust",
        "Shredded Notes", "Chipped Monocle", "Compound Seven", "Foxglove", "Placebo Tablet"
    };
    char *twinsAddons[KILLER_ADDON_COUNT] = {
        "Iridescent Pendant", "Silencing Cloth", "Drop Of Perfume", "Forest Stew", "Spinning Top",
        "Victor's Soldier", "Madeleine's Scarf", "Rusted Needle", "Sewer Sludge", "Stale Biscuit",
        "Weighty Rattle", "Baby Teeth", "Bloody Black Hood", "Cat's Eye", "Ceremonial Candelabrum",
        "Madeleine's Glove", "Cat Figurine", "Soured Milk", "Tiny Fingernail", "Toy Sword"
    };
    char *tricksterAddons[KILLER_ADDON_COUNT] = {
        "Death Throes Compliation", "Iridescent Photocard", "Cut Thru U Single", "Diamond Cufflinks", "Edge Of Revival Album",
        "Trick Blades", "Bloody Boa", "Fizz-Spin Soda", "Melodious Murder", "Ripper Brace",
        "Waiting For You Watch", "Caged Heart Shoes", "Ji-Woon's Autograph", "Lucky Blade", "On Target Single",
        "Tequila Moonrock", "Inferno Wires", "Killing Part Chords", "Memento Blades", "Trick Pouch"
    };
    char *nemesisAddons[KILLER_ADDON_COUNT] = {
        "Iridescent Umbrella Badge", "Shattered S.T.A.R.S. Badge", "Broken Recovery Coin", "Depleted Ink Ribbon", "Jill's Sandwich",
        "NE-a Parasite", "Licker Tongue", "Plant 43 Vines", "Serotonin Injector", "T-Virus Sample",
        "Tyrant Gore", "Admin Wristband", "Adrenaline Injector", "Marvin's Blood", "Mikhail's Eye",
        "Zombie Heart", "Brian's Intestine", "Damaged Syringe", "S.T.A.R.S. Field Combat Manual", "Visitor Wristband"
    };
    char *cenobiteAddons[KILLER_ADDON_COUNT] = {
        "Engineer's Fang", "Iridescent Lament Configuration", "Chatterer's Tooth", "Greasy Black Lens", "Impaling Wire",
        "Original Pain", "Frank's Heart", "Larry's Blood", "Larry's Remains", "Slice Of Frank",
        "Torture Pillar", "Flickering Television", "Liquified Gore", "Skewered Rat", "Spoiled Meal",
        "Wriggling Maggots", "Bent Nail", "Burning Candle", "Leather Strip", "Lively Crickets"
    };
    char *artistAddons[KILLER_ADDON_COUNT] = {
        "Garden Of Rot", "Iridescent Feather", "Ink Egg", "Matias' Baby Shoes", "Severed Hands",
        "Severed Tongue", "Charcoal Stick", "Darkest Ink", "O Grief, O Lover", "Silver Bell",
        "Thorny Nest", "Automatic Drawing", "Festering Carrion", "Still Life Crow", "Untitled Agony",
        "Velvet Fabric", "Choclo Corn", "Oil Paints", "Thick Tar", "Vibrant Obituary"
    };
    char *onryoAddons[KILLER_ADDON_COUNT] = {
        "Iridescent Videotape", "Remote Control", "Distorted Photo", "Tape Editing Deck", "Telephone",
        "VCR", "Bloody Fingernails", "Mother's Comb", "Rickety Pinwheel", "Ring Drawing",
        "Well Water", "Clump Of Hair", "Reiko's Watch", "Sea-Soaked Cloth", "Well Stone",
        "Yoichi's Fishing Net", "Cabin Sign", "Mother's Mirror", "Old Newspaper", "Videotape Copy"
    };
    char *dredgeAddons[KILLER_ADDON_COUNT] = {
        "Iridescent Wooden Plank", "Sacrificial Knife", "Boat Key", "Field Recorder", "Lavalier Microphone",
        "Tilling Blade", "Broken Doll", "Destroyed Pillow", "Ottomarian Writing", "War Helmet",
        "Worry Stone", "Air Freshener", "Burnt Letters", "Fallen Shingle", "Haddie's Calendar",
        "Malthinker's Skull", "Caffeine Tablets", "Follower's Cowl", "Mortar And Pestle", "Wooden Plank"
    };
    char *mastermindAddons[KILLER_ADDON_COUNT] = {
        "Iridescent Uroboros Vial", "Lab Photo", "Dark Sunglasses", "Green Herb", "Helicopter Stick",
        "Oroboros Virus", "Egg (Gold)", "Maiden Medallion", "Portable Safe", "Red Herb",
        "Video Conference Device", "Bullhorn", "Chalice (Gold)", "Leather Gloves", "Lion Medallion",
        "Loose Crank", "Jewel Beetle", "R.P.D. Shoulder Walkie", "Unicorn Medallion", "Uroboros Tendril"
    };
    char *knightAddons[KILLER_ADDON_COUNT] = {
        "Iridescent Company Banner", "Knight's Contract", "Blacksmith's Hammer", "Flint And Steel", "Healing Poultice",
        "Jailer's Chimes", "Broken Hilt", "Grim Iron Mask", "Ironworker's Tongs", "Sharpened Mount",
        "Town Watch's Torch", "Battleaxe Head", "Call To Arms", "Cold Steel Manacles", "Dried Horsemeat",
        "Treated Blade", "Gritty Lump", "Map Of The Realm", "Pillaged Mead", "Tattered Tabard"
    };
    char *skullMerchantAddons[KILLER_ADDON_COUNT] = {
        "Expired Batteries", "Iridescent Unpublished Manuscript", "Advanced Movement Prediction", "Geographical Readout", "Prototype Rotor",
        "Randomized Strobes", "Brown Noise Generator", "Infrared Upgrade", "Loose Screw", "Powdered Glass",
        "Vital Targeting Processor", "Adapative Lighting", "Low-Power Mode", "Shotgun Speakers", "Stereo Remote Mic",
        "Supercharge", "Adi Valente #1", "High Current Upgrade", "High-Power Floodlight", "Ultrasonic Speaker"
    };
    char *singularityAddons[KILLER_ADDON_COUNT] = {
        "Denied Requisition Form", "Iridescent Crystal Shard", "Crew Manifest", "Diagnostic Tool (Construction)", "Foreign Plant Fibers",
        "Soma Family Photo", "Hologram Generator", "Hyperawareness Spray", "Live Wires", "Nanomachine Gel",
        "Spent Oxygen Tank", "Android Arm", "Cremated Remains", "Cryo Gel", "Kid's Ball Glove",
        "Ultrasonic Sensor", "Broken Security Key", "Diagnostic Tool (Repair)", "Heavy Water", "Nutritional Slurry"
    };
    char *xenomorphAddons[KILLER_ADDON_COUNT] = {
        "Acidic Blood", "Improvised Cattle Prod", "Cat Carrier", "Harpoon Gun", "Self-Destruct Bolt",
        "Semiotic Keyboard", "Emergency Helmet", "Kane's Helmet", "Molted Skin", "Multipurpose Hatchet",
        "Parker's Headband", "Ash's Innards", "Brett's Cap", "Crew Headset", "Lambert's Star Map",
        "Light Wand", "Cereal Rations", "Drinking Bird", "Ovomorph", "Ripley's Watch"
    };
    char *goodGuyAddons[KILLER_ADDON_COUNT] = {
        "Hard Hat", "Iridescent Amulet", "Mirror Shards", "Pile Of Nails", "Plastic Bag",
        "Straight Razor", "Portable TV", "Rat Poison", "Running Shoes", "Silk Pillow",
        "Yardstick", "Automatic Screwdriver", "Electric Carving Knife", "Hair Spray & Candle", "Jump Rope",
        "Power Drill", "Doll Eyes", "Good Guy Box", "Strobing Light", "Tiny Scalpel"
    };
    char *unknownAddons[KILLER_ADDON_COUNT] = {
        "Captured By The Dark", "Iridescent OSS Report", "Discarded Milk Carton", "Homemade Mask", "Obscure Game Cartridge",
        "Serum Vial", "B-Movie Poster", "Footprint Cast", "Front-Page Article", "Hypnotist's Watch",
        "Vanishing Box", "Device Of Undisclosed Origin", "Last Known Recording", "Notebook Of Theories", "Slashed Backpack",
        "Victim's Map", "Blurry Photo", "Punctured Eyeball", "Rabbit's Foot", "Sketch Attempt"
    };
    char *lichAddons[KILLER_ADDON_COUNT] = {
        "Iridescent Book Of Vile Darkness", "Vorpal Sword", "Bag Of Holding", "Cloak Of Invisibility", "Dragontooth Dagger",
        "Robe Of Eyes", "Boots Of Speed", "Cloak Of Elvenkind", "Ornate Horn", "Pearl Of Power",
        "Staff Of Withering", "Glass Eye", "Lantern Of Revealing", "Potion Of Speed", "Ring Of Spell Storing",
        "Ring Of Telekinesis", "Crystal Ball", "Raven's Feather", "Tattered Headband", "Trickster's Glove"
    };
    char *darkLordAddons[KILLER_ADDON_COUNT] = {
        "Alucard's Shield", "Iridescent Ring Of Vlad", "Cube Of Zoe", "Lapis Lazuli", "Medusa's Hair",
        "Warg's Fang", "Force Of Echo", "Killer Doll", "Pocket Watch", "Sunglasses",
        "Sylph Feather", "Blood-Filled Goblet", "Magical Ticket", "Moonstone Necklace", "White Wolf Medallion",
        "Winged Boots", "Cerberus Talon", "Clock Tower Gear", "Ruby Circlet", "Traveler's Hat"
    };
    char *houndMasterAddons[KILLER_ADDON_COUNT] = {
        "Iridescent Wheel Handle", "Torn Novel", "Gunpowder Tin", "Leather Harness", "Marlinspike",
        "Ship Figurehead", "Fatty Meat", "Spiked Collar", "Training Bell", "Unfinished Map",
        "Waterskin", "Barley Meal", "Belaying Pins", "Knotted Rope", "Smoked Snapper",
        "Spyglass", "Creature's Bone", "Sticky Pitch", "Trainer's Book", "Young Coconut"
    };
    char *ghoulAddons[KILLER_ADDON_COUNT] = {
        "Iridescent Eye Patch", "Yamori's Mask", "CCG ID Card", "Fresh Coffee", "Red-Headed Centipede",
        "Torture Apparatus", "Amon's Necktie", "Aogiri Tree Robe", "Mado's Glove", "Red Spider Lily",
        "Rize's Glasses", "Blood-Stained Handkerchief", "Broken Chain", "Hide's Headphones", "Hinami's Umbrella",
        "Kaneki's Satchel", "Anteiku Apron", "Kaneki's Wallet", "Taiyaki", "The Black Goat's Egg"
    };
    char *animatronicAddons[KILLER_ADDON_COUNT] = {
        "Faz-Coin", "Iridescent Remnant", "Access Panel", "Celebrate! Poster", "Endo CPU",
        "Loot Bag", "Bonnie's Guitar Strings", "Chica's Bib", "Foxy's Hook", "Freddy's Hat",
        "Purple Guy Drawing", "Office Phone", "Party Hat", "Ripped Curtain", "Security Guard's Badge",
        "Streamers", "Greasy Paper Plate", "Help Wanted Ad", "Restaurant Menu", "Rotten Pizza"
    };
    char *krasueAddons[KILLER_ADDON_COUNT] = {
        "Chicken Head", "Shredded Gown", "Lorenza's Remains", "Mysterious Elixor", "Queen's Scepter",
        "Rotten Swine", "Dulled Knife", "Framed Newspaper", "Janjira's Hand", "Spattered Hankerchief",
        "Theater Binoculars", "Chunk Of Malai", "Crumpled Sheet Music", "Defective Metronome", "Pig's Eye",
        "Wriggling Parasite", "Broken Tiara", "First Libretto", "Luckless Mouse", "Sticky Lozenge"
    };
    //list inside of a list for killer addons, corresponds to the killer number
    char **allAddonLists[KILLER_COUNT] = {
        trapperAddons,
        wraithAddons,
        hillbillyAddons,
        nurseAddons,
        shapeAddons,
        hagAddons,
        doctorAddons,
        cannibalAddons,
        huntressAddons,
        nightmareAddons,
        pigAddons,
        clownAddons,
        spiritAddons,
        legionAddons,
        plagueAddons,
        ghostfaceAddons,
        demogorgonAddons,
        oniAddons,
        deathslingerAddons,
        executionerAddons,
        blightAddons,
        twinsAddons,
        tricksterAddons,
        nemesisAddons,
        cenobiteAddons,
        artistAddons,
        onryoAddons,
        dredgeAddons,
        mastermindAddons,
        knightAddons,
        skullMerchantAddons,
        singularityAddons,
        xenomorphAddons,
        goodGuyAddons,
        unknownAddons,
        lichAddons,
        darkLordAddons,
        houndMasterAddons,
        ghoulAddons,
        animatronicAddons,
        krasueAddons
    };
    //now realizing i could've used the same list, but i wrote killer randomizer first, but im gonna leave it be
    char *killerAddon1Rarity[KILLER_ADDON_COUNT] = {
        "Visceral", "Visceral", "Very Rare", "Very Rare", "Very Rare",
        "Very Rare", "Rare", "Rare", "Rare", "Rare",
        "Rare", "Uncommon", "Uncommon", "Uncommon", "Uncommon",
        "Uncommon", "Common", "Common", "Common", "Common",
    };
    char *killerAddon2Rarity[KILLER_ADDON_COUNT] = {
        "Visceral", "Visceral", "Very Rare", "Very Rare", "Very Rare",
        "Very Rare", "Rare", "Rare", "Rare", "Rare",
        "Rare", "Uncommon", "Uncommon", "Uncommon", "Uncommon",
        "Uncommon", "Common", "Common", "Common", "Common",
    };
    // all killer offerings with respect to its rarities
    char *killerOfferings[KILLER_OFFERING_COUNT] = {
        //page 1
        "Ebony Memento Mori", "Black Ward", "Cut Coin", "Murky Reagent", "Putrid Oak",
        "Airlock Doors", "Alien Flora", "Azarov's Key", "Beef Tallow Mixture", "Charred Wedding Photograph",
        "Crow's Eye", "Damaged Photo", "Grandma's Cookbook", "Hawkins National Laboratory I.D.", "Heart Locket",
        //page 2
        "Ichorous Loam", "Ivory Memento Mori", "Jigsaw Piece", "Macmillan's Phalanx Bone", "Mary's Letter",
        "Rotten Oak", "RPD Badge", "Shattered Bottle", "Shattered Glasses", "Strode Realty Key",
        "The Last Mask", "The Pied Piper", "Yamaoka Family Crest", "Hazy Reagent", "Moldy Oak",
        //page 3
        "Scratched Coin", "Shroud Of Vanishing", "Annotated Blueprint", "Bloodied Blueprint", "Clear Reagent",
        "Faint Reagent", "Sacrificial Ward", "Torn Blueprint", "Vigo's Blueprint"
    };
    char *killerOfferingsRarity[KILLER_OFFERING_COUNT] = {
        //page 1
        "Visceral", "Very Rare", "Very Rare", "Very Rare", "Very Rare",
        "Rare", "Rare", "Rare", "Rare", "Rare",
        "Rare", "Rare", "Rare", "Rare", "Rare",
        //page 2
        "Rare", "Rare", "Rare", "Rare", "Rare",
        "Rare", "Rare", "Rare", "Rare", "Rare",
        "Rare", "Rare", "Rare", "Uncommon", "Uncommon",
        //page 3
        "Uncommon", "Uncommon", "Common", "Common", "Common",
        "Common", "Common", "Common", "Common"
    };
    // all survivor offerings with respect to its rarities
    char *survivorOfferings[SURVIVOR_OFFERING_COUNT] = {
        //page 1
        "Murky Reagent", "Petrified Oak", "Shiny Coin", "Shroud Of Separation", "Vigo's Jar Of Salty Lips",
        "White Ward", "Airlock Doors", "Alien Flora", "Azarov's Key", "Beef Tallow Mixture",
        "Black Salt Statuette", "Charred Wedding Photograph", "Crow's Eye", "Damaged Photo", "Grandma's Cookbook",
        //page 2
        "Hawkins National Laboratory I.D.", "Heart Locket", "Ichorous Loam", "Ivory Chalk Pouch", "Jigsaw Piece",
        "Macmillan's Phalanx Bone", "Mary's Letter", "RPD Badge", "Shattered Bottle", "Shattered Glasses",
        "Strode Realty Key", "The Last Mask", "The Pied Piper", "Yamaoka Family Crest", "Cream Chalk Pouch",
        //page 3
        "Hazy Reagent", "Salt Pouch", "Shroud Of Union", "Tarnished Coin", "Vigo's Shroud",
        "Annotated Blueprint", "Bloodied Blueprint", "Chalk Pouch", "Clear Reagent", "Faint Reagent",
        "Sacrificial Ward", "Torn Blueprint", "Vigo's Blueprint"
    };
    char *survivorOfferingsRarity[SURVIVOR_OFFERING_COUNT] = {
        //page 1
        "Very Rare", "Very Rare", "Very Rare", "Very Rare", "Very Rare",
        "Very Rare", "Rare", "Rare", "Rare", "Rare",
        "Rare", "Rare", "Rare", "Rare", "Rare",
        //page 2
        "Rare", "Rare", "Rare", "Rare", "Rare",
        "Rare", "Rare", "Rare", "Rare", "Rare",
        "Rare", "Rare", "Rare", "Rare", "Uncommon",
        //page 3
        "Uncommon", "Uncommon", "Uncommon", "Uncommon", "Uncommon",
        "Common", "Common", "Common", "Common", "Common",
        "Common", "Common", "Common"
    };
    // seeding the number generator
    srand(time(NULL));
    // terminal main menu
    printf("Dead by Daylight Randomizer 9.3.0.\n");
    printf("By DoggieSquad\n");
    printf("1. Killer\n");
    printf("2. Survivor\n");
    printf("3. Exit\n");
    printf("Choose an option by typing in a number: ");
    scanf("%d", &option);
    switch (option) {
        case 1: {
            // killer option
            printf("You chose the killer role option.\n");

            printf("Random Killer? Type 1 for yes, 0 for no: ");
            scanf("%d", &randKiller);
            if (randKiller) {
                //to-do add separate randomize addons for a selected killer
                printf("Random Addons for this Killer? Type 1 for yes, 0 for no: ");
                scanf("%d", &randKillerAddon);
            }
            printf("Random Perks? Type 1 for yes, 0 for no: ");
            scanf("%d", &randKillerPerks);

            printf("Random Offering? Type 1 for yes, 0 for no: ");
            scanf("%d", &randKillerOffering);

            //failsafe
            if (randKiller != 1 && randKillerPerks != 1 && randKillerOffering != 1) {
                printf("You chose not to randomize a killer, perks, or offering.\n");
                system("pause");
                return 0;
            }
            
            // starts outputting everything you selected or wanted
            printf("Here is your random selection:\n");
            if (randKiller) {

                killer = rand() % KILLER_COUNT;
                printf("Killer: %s%s%s\n", COLOR_KILLER, killerList[killer], RESET);

                //called if randKillerAddon is 1 otherwise it prints just the killer
                //do while loop to make sure the second addon is unique
                if (randKillerAddon && allAddonLists[killer][0]) {
                    a1 = rand() % KILLER_ADDON_COUNT;
                do { a2 = rand() % KILLER_ADDON_COUNT; 
                } while (a2 == a1);

                //calls on the color rarity function all the way up top
                const char *col1 = rarityColor(killerAddon1Rarity[a1]);
                const char *col2 = rarityColor(killerAddon2Rarity[a2]);

                printf("Addons: %s%s%s (%s)%s  &  %s%s%s (%s)%s\n", 
                    col1, allAddonLists[killer][a1], RESET, //color, megalistofaddons[killer number][addon number 1], RESET
                    killerAddon1Rarity[a1], RESET, //rarity of addon in ()
                    col2, allAddonLists[killer][a2], RESET, //color, megalistofaddons[killer number][addon number 2], RESET
                    killerAddon2Rarity[a2], RESET); //rarity of addon in ()
                }
            }
            if (randKillerPerks) {
                // while loop to make sure all 4 perks are unique and don't repeat
                //to-do add an option to select a certain amount of perks i.e. 1-3
                count = 0;
                while (count < 4) {
                    int p = rand() % PERK_KILLER_COUNT, dup = 0;
                    for (int j = 0; j < count; j++)
                    if (selected[j] == p) { 
                        dup = 1; 
                        break; 
                    }
                    if (!dup) 
                        selected[count++] = p;
                }
                for (int i = 0; i < 4; i++)
                    printf("Perk %d: %s%s%s\n", 
                        i+1, COLOR_PERK, perkKillerList[selected[i]], RESET); // #number, Color, selected perk, RESET 
            }
            
            //offerings offerings offerings, just a mix of code between picking a killer and getting the rarity color
            if (randKillerOffering) {
                offering = rand() % KILLER_OFFERING_COUNT;
                const char *colOffering = rarityColor(killerOfferingsRarity[offering]);
                printf("Offering: %s%s%s (%s)%s \n", 
                    colOffering, killerOfferings[offering], RESET, //COLOR, the offering itself, RESET
                    killerOfferingsRarity[offering], RESET); //string of the rarity, RESET
            }

            // ID system that i want to work on later
            /* if (randKiller && randKillerAddon && randKillerPerks && randKillerOffering) {
                printf("\nID can share with others!\n");
                printf("Otherwise it is saved inside of a txt file.\n");
                printf("\nK%02d-%02d%02d-%03d%03d%03d%03d-%02d\n", killer, a1, a2, selected[0], selected[1], selected[2], selected[3], offering);
                FILE *fp = fopen(HISTORY_FILE, "a");
                if (!fp) { 
                    printf("Cannot open save file.\n"); 
                    system("pause");
                    return 0; 
                }
                fprintf(fp, "K%02d-%02d%02d-%03d%03d%03d%03d-%02d\n", killer, a1, a2, selected[0], selected[1], selected[2], selected[3], offering);
                fclose(fp);
            } */

            printf("\nGood luck in your match!\n");
            system("pause");
            return 0;
        }
    
        case 2: {
            //survivor option
            printf("You chose the survivor role option.\n");

            printf("Random Survivor? Type 1 for yes, 0 for no: ");
            scanf("%d", &randSurvivor);

            printf("Random Item? Type 1 for yes, 0 for no: ");
            scanf("%d", &randSurvivorItem);
            if (randSurvivorItem) {
                // add later on a separate option to give addons for a selected item
                printf("Random Addons for this item? Type 1 for yes, 0 for no: ");
                scanf("%d", &randSurvivorAddon);
            }

            printf("Random Perks? Type 1 for yes, 0 for no: ");
            scanf("%d", &randSurvivorPerks);

            printf("Random Offering? Type 1 for yes, 0 for no :");
            scanf("%d", &randSurvivorOffering);

            //failsafe
            if(randSurvivor != 1 && randSurvivorItem != 1 && randSurvivorPerks != 1 && randSurvivorOffering != 1 && randSurvivorOffering != 1) {
                printf("You chose not to randomize a survivor, item, perks, or offering.");
                system("pause");
                return 0;
            }
            
            printf("Here is your random selection:\n");
            // entire sequence yet again
            if(randSurvivor) {
                survivor = rand() % SURVIVOR_COUNT;
                printf("Survivor: %s%s%s\n", COLOR_SURVIVOR, survivorList[survivor], RESET);
            }

            if (randSurvivorItem) {
                item = rand() % SURVIVOR_ITEM_COUNT;
                subItem = rand() % survivorItemCount[item];
                const char *colItem = rarityColor(survivorItemRarity[item][subItem]);
                printf("Item: %s%s%s (%s)%s\n", 
                    colItem, survivorItem[item][subItem], RESET, //COLOR, string of the item, RESET
                    survivorItemRarity[item][subItem], RESET); //string of color, RESET
                
                //why why why why why
                if (randSurvivorAddon) {
                int addonCount = survivorItemAddonCount[item];
                addon1 = rand() % addonCount;
                do { 
                    addon2 = rand() % addonCount; 
                } while (addon2 == addon1);
                const char *colItemAdd1 = rarityColor(survivorItemAddonRarity[item][addon1]);
                const char *colItemAdd2 = rarityColor(survivorItemAddonRarity[item][addon2]);
                printf("Addons: %s%s%s (%s)%s & %s%s%s (%s)%s\n",
                colItemAdd1, survivorItemAddons[item][addon1], RESET, //COLOR, STRING OF ITEM ADDON LIST INSIDE OF A LIST, RESET
                survivorItemAddonRarity[item][addon1], RESET,  //STRING OF RARITY
                colItemAdd2, survivorItemAddons[item][addon2], RESET, //COLOR, STRING OF 2ND ITEM ADDON MORE LIST INSIDE OF LIST
                survivorItemAddonRarity[item][addon2], RESET); //STRING OF RARITY, GOODBYE
                }
            }

            if (randSurvivorPerks) {
                //copied from the killer perk randomizer, but works the same way, no need to change
                count = 0;
                while (count < 4) {
                    int p = rand() % PERK_SURVIVOR_COUNT, dup = 0;
                    for (int j = 0; j < count; j++)
                    if (selected[j] == p) { dup = 1; break; }
                    if (!dup) selected[count++] = p;
                }
                for (int i = 0; i < 4; i++)
                    printf("Perk %d: %s%s%s\n", i+1, COLOR_PERK, perkSurvivorList[selected[i]], RESET);
            }

            if (randSurvivorOffering) {
                // very similar to randKillerOffering
                offering = rand() % SURVIVOR_OFFERING_COUNT;
                const char *colOffering = rarityColor(survivorOfferingsRarity[offering]);
                printf("Offering: %s%s%s (%s)%s\n", colOffering, survivorOfferings[offering], RESET, survivorOfferingsRarity[offering], RESET);
            }

            //for later
            /*if (randSurvivor && randSurvivorItem && randSurvivorAddon && randSurvivorPerks && randSurvivorOffering) {
                printf("\nID can share with others!\n");
                printf("\nS%02d-%02d-%02d-%02d%02d-%03d%03d%03d%03d-%02d\n",
                survivor,      // which survivor
                item,          // item-type
                subItem,       // which specific flashlight/medkit/…
                addon1, addon2,// the two addons
                selected[0], selected[1], selected[2], selected[3], // perks
                offering);     // offering

                FILE *fp = fopen(HISTORY_FILE, "a");
                if (!fp) { 
                    printf("Cannot open save file.\n"); 
                    system("pause");
                    return 0; 
                }
                fprintf(fp,  "S%02d-%02d-%02d-%02d%02d-%03d%03d%03d%03d-%02d\n",
                survivor, item, subItem, addon1, addon2, selected[0], selected[1], selected[2], selected[3], offering);
                fclose(fp);
            } */

            printf("\nGood luck in your match!\n");
            system("pause");
            return 0;
        }
        case 3: {
            //exits the program
            printf("Goodbye!\n");
            system("pause");
            return 0;
        }
    }
    //failsafe
    printf("Invalid option selected. Please restart the program and choose a valid option.\n");
    system("pause");
    return 0;
}
//DBD RANDOMIZER BY DOGGIESQUAD 11/28/2025
//PLEASE LET ME KNOW IF THERE ARE ANY BUGS OR MISPELLINGS
//EVERYTHING IS OWNED BY BHVR, I OWN NOTHING