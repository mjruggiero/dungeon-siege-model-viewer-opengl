// utilities.h

#ifndef _UTILITIES_H_
#define _UTILITIES_H_

char *AddPathToFilename(const char *filename)
{
	int i;
	int numKeys = 0;
	for(i = 0; i < strlen(filename); ++i)
	{
		if(filename[i] == '_')
			++numKeys;
	}
	++numKeys;

	char keys[8][32];
	//keys = new char[numKeys][4];
	
	const char *pStart = filename;
	const char *pEnd = filename;
	for(i = 0; i < numKeys; ++i)
	{
		while(*pEnd != '_' && *pEnd != '.')
			++pEnd;
		strncpy(keys[i], pStart, pEnd - pStart);
		keys[i][pEnd - pStart] = '\0';
		pStart = ++pEnd;
	}
	
	for(i = 0; i < numKeys; ++i)
	{
		// meshes
		if(strcmp(keys[i], "m") == 0)
			strcpy(keys[i], "meshes/");

			// weapons
			else if(strcmp(keys[i], "w") == 0)
				strcpy(keys[i], "weapons/");
			else if(strcmp(keys[i], "arr") == 0)
				strcpy(keys[i], "arrow/");
			else if(strcmp(keys[i], "axe") == 0)
				strcpy(keys[i], "axe/");
			else if(strcmp(keys[i], "bow") == 0)
				strcpy(keys[i], "bow/");
			else if(strcmp(keys[i], "clb") == 0)
				strcpy(keys[i], "club/");
			else if(strcmp(keys[i], "dag") == 0)
				strcpy(keys[i], "dagger/");
			else if(strcmp(keys[i], "hmr") == 0)
				strcpy(keys[i], "hammer/");
			else if(strcmp(keys[i], "mce") == 0)
				strcpy(keys[i], "mace/");
			else if(strcmp(keys[i], "stf") == 0)
				strcpy(keys[i], "staff/");
			else if(strcmp(keys[i], "swd") == 0)
				strcpy(keys[i], "sword/");
			else if(strcmp(keys[i], "misc") == 0)
				strcpy(keys[i], "misc/");
			else if(strcmp(keys[i], "csb") == 0)
				strcpy(keys[i], "crossbow/");

			// armor
			else if(strcmp(keys[i], "a") == 0)
				strcpy(keys[i], "armor/");
			else if(strcmp(keys[i], "suit") == 0)
				strcpy(keys[i], "suits/");
			else if(strcmp(keys[i], "shd") == 0)
				strcpy(keys[i], "shields/");
			else if(strcmp(keys[i], "boot") == 0)
				strcpy(keys[i], "boots/");
			else if(strcmp(keys[i], "gntl") == 0)
				strcpy(keys[i], "gauntlets/");
			else if(strcmp(keys[i], "hlmt") == 0)
				strcpy(keys[i], "helmets/");

			// items
			else if(strcmp(keys[i], "csl") == 0)
				strcpy(keys[i], "castle/");
			else if(strcmp(keys[i], "glb") == 0)
				strcpy(keys[i], "global/");
			else if(strcmp(keys[i], "grs") == 0)
				strcpy(keys[i], "grass/");
			else if(strcmp(keys[i], "cav") == 0)
				strcpy(keys[i], "caves/");
			else if(strcmp(keys[i], "frm") == 0)
				strcpy(keys[i], "farm/");
			else if(strcmp(keys[i], "lav") == 0)
				strcpy(keys[i], "lava/");
			else if(strcmp(keys[i], "gob") == 0)
				strcpy(keys[i], "goblin_inv/");
			else if(strcmp(keys[i], "ice") == 0)
				strcpy(keys[i], "ice/");
			else if(strcmp(keys[i], "des") == 0)
				strcpy(keys[i], "desert/");
			else if(strcmp(keys[i], "swp") == 0)
				strcpy(keys[i], "swamp/");

			// sfx
			else if(strcmp(keys[i], "sfx") == 0)
				strcpy(keys[i], "sfx/");
			else if(strcmp(keys[i], "mag") == 0)
				strcpy(keys[i], "magic/");

			// characters
			else if(strcmp(keys[i], "c") == 0)
				strcpy(keys[i], "characters/");
			
			// evil a monsters
			else if(strcmp(keys[i], "eam") == 0)
				strcpy(keys[i], "evil_a_monsters/");
				else if(strcmp(keys[i], "dg") == 0)
					strcpy(keys[i], "droog/");
				else if(strcmp(keys[i], "dga") == 0)
					strcpy(keys[i], "droog_archer/");
				else if(strcmp(keys[i], "dgg") == 0)
					strcpy(keys[i], "droog_grunt/");
				else if(strcmp(keys[i], "dgm") == 0)
					strcpy(keys[i], "droog_mage/");
				else if(strcmp(keys[i], "ggt") == 0)
					strcpy(keys[i], "goblin_grunt/");
				else if(strcmp(keys[i], "ggg") == 0)
					strcpy(keys[i], "goblin_guard/");
				else if(strcmp(keys[i], "kc") == 0)
					strcpy(keys[i], "krug_commander/");
				else if(strcmp(keys[i], "kg") == 0)
					strcpy(keys[i], "krug_grunt/");
				else if(strcmp(keys[i], "kr") == 0)
					strcpy(keys[i], "krug_raider/");
				else if(strcmp(keys[i], "ks") == 0)
					strcpy(keys[i], "krug_shaman/");
				else if(strcmp(keys[i], "sa") == 0)
					strcpy(keys[i], "seck_archer/");
				else if(strcmp(keys[i], "sc") == 0)
					strcpy(keys[i], "seck_commander/");
				else if(strcmp(keys[i], "sg") == 0)
					strcpy(keys[i], "seck_grunt/");
				else if(strcmp(keys[i], "sm") == 0)
					strcpy(keys[i], "seck_mage/");

			// evil c monsters
			else if(strcmp(keys[i], "ecm") == 0)
				strcpy(keys[i], "evil_c_monsters/");
				else if(strcmp(keys[i], "sk") == 0)
					strcpy(keys[i], "skeleton/");
				else if(strcmp(keys[i], "cy") == 0)
					strcpy(keys[i], "cyclops/");
				else if(strcmp(keys[i], "db") == 0)
					strcpy(keys[i], "darkblood/");
				else if(strcmp(keys[i], "fe") == 0)
					strcpy(keys[i], "fire_elemental/");
				else if(strcmp(keys[i], "ga") == 0)
					strcpy(keys[i], "gorack/");
				else if(strcmp(keys[i], "kl") == 0)
					strcpy(keys[i], "kell/");
				else if(strcmp(keys[i], "bk") == 0)
					strcpy(keys[i], "braak/");
				else if(strcmp(keys[i], "md") == 0)
					strcpy(keys[i], "memaad/");
				else if(strcmp(keys[i], "skg") == 0)
					strcpy(keys[i], "skeleton_guard/");
				else if(strcmp(keys[i], "skh") == 0)
					strcpy(keys[i], "skeleton_horse/");
				else if(strcmp(keys[i], "to") == 0)
					strcpy(keys[i], "toreck/");
				else if(strcmp(keys[i], "tg") == 0)
					strcpy(keys[i], "trog/");
				else if(strcmp(keys[i], "wr") == 0)
					strcpy(keys[i], "Wraith/");
			
			
			// good a heroes
			else if(strcmp(keys[i], "gah") == 0)
				strcpy(keys[i], "good_a_heroes/");
				else if(strcmp(keys[i], "fb") == 0)
					strcpy(keys[i], "farmboy/");
				else if(strcmp(keys[i], "fg") == 0)
					strcpy(keys[i], "farmgirl/");
			
			// good a npc
			else if(strcmp(keys[i], "gan") == 0)
				strcpy(keys[i], "good_a_npc/");
				else if(strcmp(keys[i], "df") == 0)
					strcpy(keys[i], "dwarf/");
			
			// good b npc
			else if(strcmp(keys[i], "gbn") == 0)
				strcpy(keys[i], "good_b_npc/");
				else if(strcmp(keys[i], "bk") == 0)
					strcpy(keys[i], "barkeep/");
				else if(strcmp(keys[i], "bs") == 0)
					strcpy(keys[i], "blacksmith/");
				else if(strcmp(keys[i], "fy") == 0)
					strcpy(keys[i], "fairy/");
				else if(strcmp(keys[i], "ft") == 0)
					strcpy(keys[i], "foretuneteller/");
				else if(strcmp(keys[i], "ja") == 0)
					strcpy(keys[i], "jeriah/");
				else if(strcmp(keys[i], "kg") == 0)
					strcpy(keys[i], "king/");
				else if(strcmp(keys[i], "mk") == 0)
					strcpy(keys[i], "merik/");
				else if(strcmp(keys[i], "pmo") == 0)
					strcpy(keys[i], "peasant_m_old/");
				else if(strcmp(keys[i], "psm") == 0)
					strcpy(keys[i], "peasant_m/");
				else if(strcmp(keys[i], "psf") == 0)
					strcpy(keys[i], "peasant_f/");
				else if(strcmp(keys[i], "px") == 0)
					strcpy(keys[i], "pixie/");
				else if(strcmp(keys[i], "tfm") == 0)
					strcpy(keys[i], "townfolk_m/");
				else if(strcmp(keys[i], "tff") == 0)
					strcpy(keys[i], "townfolk_f/");


			// neutral ambients
			else if(strcmp(keys[i], "na") == 0)
				strcpy(keys[i], "neutral_ambients/");
				else if(strcmp(keys[i], "bs") == 0)
					strcpy(keys[i], "baby_spider/");
				else if(strcmp(keys[i], "br") == 0)
					strcpy(keys[i], "bear/");
				else if(strcmp(keys[i], "bt") == 0)
					strcpy(keys[i], "bat/");
				else if(strcmp(keys[i], "bd") == 0)
					strcpy(keys[i], "bird/");
				else if(strcmp(keys[i], "bf") == 0)
					strcpy(keys[i], "butterfly/");
				else if(strcmp(keys[i], "ckn") == 0)
					strcpy(keys[i], "chicken/");
				else if(strcmp(keys[i], "cw") == 0)
					strcpy(keys[i], "cow/");
				else if(strcmp(keys[i], "cb") == 0)
					strcpy(keys[i], "crab/");
				else if(strcmp(keys[i], "dr") == 0)
					strcpy(keys[i], "deer/");
				else if(strcmp(keys[i], "dg") == 0)
					strcpy(keys[i], "dog/");
				else if(strcmp(keys[i], "dg2") == 0)
					strcpy(keys[i], "dog2/");
				else if(strcmp(keys[i], "fw") == 0)
					strcpy(keys[i], "fawn/");
				else if(strcmp(keys[i], "fi") == 0)
					strcpy(keys[i], "fish/");
				else if(strcmp(keys[i], "pm") == 0)
					strcpy(keys[i], "pack_mule/");
				else if(strcmp(keys[i], "pg") == 0)
					strcpy(keys[i], "pig/");
				else if(strcmp(keys[i], "ra") == 0)
					strcpy(keys[i], "raven/");
				else if(strcmp(keys[i], "rt") == 0)
					strcpy(keys[i], "rat/");
				else if(strcmp(keys[i], "sg") == 0)
					strcpy(keys[i], "seagull/");
				

		// bitmaps
		else if(strcmp(keys[i], "b") == 0)
			strcpy(keys[i], "bitmaps/");
		
		// animations
		else if(strcmp(keys[i], "a") == 0)
			strcpy(keys[i], "animations/");
		
		else
			strcpy(keys[i], "");
	}

	char *result = new char[1024];
	if(strcmp(keys[0], "") != 0)
		strcpy(result, keys[0]);
	for(i = 1; i < numKeys; ++i)
	{
		if(strcmp(keys[1], "") != 0)
			strcat(result, keys[i]);
	}

	strcat(result, filename);

	return result;
}


#endif // _UTILITIES_H_
