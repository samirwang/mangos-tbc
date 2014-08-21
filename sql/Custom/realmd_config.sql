/*
Navicat MySQL Data Transfer

Source Server         : Local
Source Server Version : 50617
Source Host           : localhost:3306
Source Database       : tbc_realmd

Target Server Type    : MYSQL
Target Server Version : 50617
File Encoding         : 65001

Date: 2014-08-21 16:16:18
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for config
-- ----------------------------
DROP TABLE IF EXISTS `config`;
CREATE TABLE `config` (
  `realm` tinyint(2) unsigned NOT NULL,
  `name` varchar(255) COLLATE utf8_unicode_ci NOT NULL,
  `value` varchar(255) COLLATE utf8_unicode_ci DEFAULT NULL,
  `description` varchar(255) COLLATE utf8_unicode_ci DEFAULT NULL,
  PRIMARY KEY (`realm`,`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- ----------------------------
-- Records of config
-- ----------------------------
INSERT INTO `config` VALUES ('1', 'ActivateWeather', '1', 'Activate weather system\r\nDefault: 1 (true)\r\n         0 (false)');
INSERT INTO `config` VALUES ('1', 'AddonChannel', '1', 'Permit/disable the use of the addon channel through the server\r\n(some client side addons can stop work correctly with disabled addon channel)\r\nDefault: 1 (permit addon channel)\r\n         0 (do not permit addon channel)');
INSERT INTO `config` VALUES ('1', 'AllFlightPaths', '0', 'Players will start with all flight paths (Note: ALL flight paths, not only player\'s team)\r\nDefault: 0 (true)\r\n         1 (false)');
INSERT INTO `config` VALUES ('1', 'AllowTwoSide.Accounts', '0', 'Allow accounts to create characters in both teams in any game type.\r\nDefault: 0 (Not allowed)\r\n         1 (Allowed)');
INSERT INTO `config` VALUES ('1', 'AllowTwoSide.AddFriend', '0', 'Allow adding friends from other team in friend list.\r\nDefault: 0 (Not allowed)\r\n         1 (Allowed)');
INSERT INTO `config` VALUES ('1', 'AllowTwoSide.Interaction.Auction', '0', 'Merge horde and alliance aution houses\r\nDefault: 0 (Disabled)\r\n         1 (Enabled)');
INSERT INTO `config` VALUES ('1', 'AllowTwoSide.Interaction.Channel', '0', 'Merge chat channels for both teams\r\nDefault: 0 (Enabled)\r\n         1 (Disabled)');
INSERT INTO `config` VALUES ('1', 'AllowTwoSide.Interaction.Chat', '0', 'Allow people to chat with eachother across teams\r\nDefault: 0 (Not allowed)\r\n         1 (Allowed)');
INSERT INTO `config` VALUES ('1', 'AllowTwoSide.Interaction.Group', '0', 'Allow different teams to be in groups with eachother\r\nDefault: 0 (Not allowed)\r\n         1 (Allowed)');
INSERT INTO `config` VALUES ('1', 'AllowTwoSide.Interaction.Guild', '0', 'Allow horde and alliance to be in the same guild\r\nDefault: 0 (Not allowed)\r\n         1 (Allowed)');
INSERT INTO `config` VALUES ('1', 'AllowTwoSide.Interaction.Mail', '0', 'Allow mailing between horde and alliance\r\nDefault: 0 (Not allowed)\r\n         1 (Allowed)');
INSERT INTO `config` VALUES ('1', 'AllowTwoSide.WhoList', '0', 'Enable horde and alliance to see eachother on who list\r\nDefault: 0 (Disabled)\r\n         1 (Enabled)');
INSERT INTO `config` VALUES ('1', 'AlwaysMaxSkillForLevel', '0', 'Players will automatically gain max level dependent (weapon/defense) skill when logging in, leveling up etc\r\nDefault: 0 (false)\r\n         1 (true)');
INSERT INTO `config` VALUES ('1', 'AntiCheat.SpeedCheckInterval', '500', 'How often in milliseconds to check for speedcheat lower gives less slightly higher failrate\r\nDefault: 500 - Wait 500 server milliseconds between checks\r\n         0   - Check every packet');
INSERT INTO `config` VALUES ('1', 'AntiCheat.SpeedCheckTolerance', '1', 'How many % overtravel is allowed\r\nDefault: 1   - 1% overtravel allowed\r\n         0   - Will fail a lot');
INSERT INTO `config` VALUES ('1', 'Arena.ArenaSeason.ID', '1', 'Current area season id, used for selection some rated arena settings.\r\nDefault: 1');
INSERT INTO `config` VALUES ('1', 'Arena.AutoDistributeInterval', '7', 'How often should the distribution take place\nIf automatic distribution is enabled in days\nDefault: 7 (weekly)');
INSERT INTO `config` VALUES ('1', 'Arena.AutoDistributePoints', '0', 'Set if arena points should be distributed automatically, or by GM command\nDefault: 0 (disable) (recommended): use gm command or sql query to distribute the points\n         1 (enable)  arena points are distributed automatically');
INSERT INTO `config` VALUES ('1', 'Arena.MaxRatingDifference', '150', 'The maximum rating difference between two groups in rated matches\nDefault: 0 (disable, rating difference is discarded)');
INSERT INTO `config` VALUES ('1', 'Arena.QueueAnnouncer.Exit', '0', 'Enable arena queue announcer posting to chat at exit\nDefault: 0 (disable)\n         1 (enable)');
INSERT INTO `config` VALUES ('1', 'Arena.QueueAnnouncer.Join', '0', 'Enable arena queue announcer posting to chat at join\nDefault: 0 (disable)\n         1 (enable)');
INSERT INTO `config` VALUES ('1', 'Arena.RatingDiscardTimer', '600000', 'After the specified milliseconds has passed,\nrating information will be discarded when selecting teams for matches\nalso initiates an update by this timer\nDefault: 600000 (10 minutes, recommended)\n         0 (disable)');
INSERT INTO `config` VALUES ('1', 'Arena.StartPersonalRating', '-1', 'Default Personal Rating at team creation\nDefault: -1 (use season dependent default value)');
INSERT INTO `config` VALUES ('1', 'Arena.StartRating', '-1', 'Default Rating at team creation\nDefault: -1 (use season dependent default value)');
INSERT INTO `config` VALUES ('1', 'Auction.Deposit.Min', '0', 'Minimum auction deposit size in copper\nDefault: 0');
INSERT INTO `config` VALUES ('1', 'Battleground.CastDeserter', '1', 'Cast Deserter spell at player who leave battleground in progress\nDefault: 1 (enable)\n         0 (disable)');
INSERT INTO `config` VALUES ('1', 'Battleground.InvitationType', '0', 'Set Battleground invitation type\nDefault: 0 (normal - invite as much players to bg as possible, don\'t bother with ballance)\n         1 (Experimental - don\'t allow to invite much more players of one faction)');
INSERT INTO `config` VALUES ('1', 'BattleGround.PremadeGroupWaitForMatch', '1800000', 'The time in which premade group of 1 faction waits in BG Queue for premade group of other faction\nDefault: 1800000 (30 minutes)\n         0 - disable premade group matches (group always added to bg team in normal way)');
INSERT INTO `config` VALUES ('1', 'BattleGround.PrematureFinishTimer', '300000', 'The time to end the bg if there are less than MinPlayersPerTeam on one side in milliseconds\nDefault: 300000 (5 minutes)\n         0 - disable (not recommended)');
INSERT INTO `config` VALUES ('1', 'Battleground.QueueAnnouncer.Join', '0', 'Enable queue announcer posting to chat at join\nDefault: 0 (not send)\n         1 (send to joined player only)\n         2 (send to all players)');
INSERT INTO `config` VALUES ('1', 'Battleground.QueueAnnouncer.Start', '0', 'Enable queue announcer posting to chat at BG start\nDefault: 0 (disable)\n         1 (enable)');
INSERT INTO `config` VALUES ('1', 'BeepAtStart', '1', 'Beep at mangosd start finished\nDefault: 1 (true)\n         0 (false)');
INSERT INTO `config` VALUES ('1', 'CastUnstuck', '1', 'Allow cast Unstuck spell at .start or client Help option use\nDefault: 1 (true)\n         0 (false)');
INSERT INTO `config` VALUES ('1', 'ChangeWeatherInterval', '600000', 'Weather update interval (in milliseconds)\nDefault: 600000 (10 min)');
INSERT INTO `config` VALUES ('1', 'Channel.RestrictedLfg', '1', 'Restrict use LookupForGroup channel only registered in LFG tool players\nDefault: 1 (allow join to channel only if active in LFG)\n         0 (allow join to channel in any time)');
INSERT INTO `config` VALUES ('1', 'Channel.SilentlyGMJoin', '0', 'Silently join GM characters (security level > 1) to channels\nDefault: 0 (join announcement in normal way)\n         1 (GM join without announcement)');
INSERT INTO `config` VALUES ('1', 'CharactersCreatingDisabled', '0', 'Disable characters creating for specific team or any (non-player accounts not affected)\nDefault: 0 - enabled\n         1 - disabled only for Alliance\n         2 - disabled only for Horde\n         3 - disabled for both teams');
INSERT INTO `config` VALUES ('1', 'CharactersPerAccount', '50', 'Limit numbers of characters per account (at all realms).\nNote: this setting limits the character creating at the _current_ realm base at characters amount at all realms\nDefault: 50\n        The number must be >= CharactersPerRealm');
INSERT INTO `config` VALUES ('1', 'CharactersPerRealm', '10', 'Limit numbers of characters for account at realm\nDefault: 10 (client limitation)\n        The number must be between 1 and 10');
INSERT INTO `config` VALUES ('1', 'CharDelete.KeepDays', '30', 'Define the amount of days for which the characters are kept in the database before\nthey will be removed\nDefault: 30\n         0  - Don\'t delete any characters, they stay in the database forever.');
INSERT INTO `config` VALUES ('1', 'CharDelete.Method', '0', 'Character deletion behavior\nDefault: 0  - Completely remove the character from the database\n         1  - Unlinking, the character gets unlinked from the account,\n              the name gets freed up and appears as deleted ingame');
INSERT INTO `config` VALUES ('1', 'CharDelete.MinLevel', '0', 'Character gets deleted by CharDelete.Method=0 when the character\nhasn\'t the specified level yet.\nDefault: 0  - For all characters the specified mode will be used\n         1+ - Only for players which have reached the specified level\n              will be d');
INSERT INTO `config` VALUES ('1', 'ChatFakeMessagePreventing', '0', 'Chat protection from creating fake messages using a lot spaces (other invisible symbols),\nnot applied to addon language messages, but can prevent working old addons\nthat use normal languages for sending data to another clients.\nDefault: 0 (disable fake me');
INSERT INTO `config` VALUES ('1', 'ChatFlood.MessageCount', '10', 'Chat anti-flood protection, haste message count to activate protection\nDefault: 10\n         0 (disable anti-flood protection)');
INSERT INTO `config` VALUES ('1', 'ChatFlood.MessageDelay', '1', 'Chat anti-flood protection, minimum message delay to count message\nDefault: 1 (in secs)');
INSERT INTO `config` VALUES ('1', 'ChatFlood.MuteTime', '10', 'Chat anti-flood protection, mute time at activation flood protection (not saved)\nDefault: 10 (in secs)');
INSERT INTO `config` VALUES ('1', 'ChatStrictLinkChecking.Kick', '0', 'Defines, what should be done if a message is considered to contain invalid pipe commands.\nDefault: 0 (silently ignore message)\n         1 (kick players who sent invalid formed messages)');
INSERT INTO `config` VALUES ('1', 'ChatStrictLinkChecking.Severity', '0', 'Check chat messages for ingame links to spells, items, quests, achievements etc.\nDefault: 0 (disable link checking)\n         1 (check if only valid pipe commands are used. This prevents posting pictures for example)\n         2 (verifiy that pipe commands ');
INSERT INTO `config` VALUES ('1', 'CleanCharacterDB', '1', 'Perform character db cleanups on start up\nDefault: 1 (Enable)\n         0 (Disabled)');
INSERT INTO `config` VALUES ('1', 'Compression', '1', 'Compression level for update packages sent to client (1..9)\nDefault: 1 (speed)\n         9 (best compression)');
INSERT INTO `config` VALUES ('1', 'ConfVersion', '2013012201', 'Config version');
INSERT INTO `config` VALUES ('1', 'Console.Enable', '1', 'Enable console\nDefault: 1 - on\n         0 - off');
INSERT INTO `config` VALUES ('1', 'Corpse.Decay.ELITE', '600', 'Seconds until creature corpse will decay without being looted or skinned (not used when creature does not have loot initially)\nDefault: 300, 900, 600, 1200, 3600');
INSERT INTO `config` VALUES ('1', 'Corpse.Decay.NORMAL', '300', 'Seconds until creature corpse will decay without being looted or skinned (not used when creature does not have loot initially)\r\nDefault: 300, 900, 600, 1200, 3600');
INSERT INTO `config` VALUES ('1', 'Corpse.Decay.RARE', '900', 'Seconds until creature corpse will decay without being looted or skinned (not used when creature does not have loot initially)\nDefault: 300, 900, 600, 1200, 3600');
INSERT INTO `config` VALUES ('1', 'Corpse.Decay.RAREELITE', '1200', 'Seconds until creature corpse will decay without being looted or skinned (not used when creature does not have loot initially)\nDefault: 300, 900, 600, 1200, 3600');
INSERT INTO `config` VALUES ('1', 'Corpse.Decay.WORLDBOSS', '3600', 'Seconds until creature corpse will decay without being looted or skinned (not used when creature does not have loot initially)\nDefault: 300, 900, 600, 1200, 3600');
INSERT INTO `config` VALUES ('1', 'Corpse.EmptyLootShow', '1', 'If target can have loot (or can be skining after loot) but no loot generated still show loot window\nDefault: 1 (show)\n         0 (not show)');
INSERT INTO `config` VALUES ('1', 'CreatureFamilyAssistanceDelay', '1500', 'Reaction time for creature assistance call\nDefault: 1500 (1.5s)');
INSERT INTO `config` VALUES ('1', 'CreatureFamilyAssistanceRadius', '10.000000', 'Radius which creature will use to call assistance without moving\nDefault: 10\n         0   - off');
INSERT INTO `config` VALUES ('1', 'CreatureFamilyFleeAssistanceRadius', '30.000000', 'Radius which creature will use to seek for a near creature for assistance. Creature will flee to this creature.\nDefault: 30\n         0   - off');
INSERT INTO `config` VALUES ('1', 'CreatureFamilyFleeDelay', '7000', 'Time during which creature can flee when no assistant found\nDefault: 7000 (7s)');
INSERT INTO `config` VALUES ('1', 'CreatureRespawnAggroDelay', '5000', 'The delay between when a creature spawns and when it can be aggroed by nearby movement.\nDefault: 5000 (5s)');
INSERT INTO `config` VALUES ('1', 'CrossBattlegrounds.Enabled', '1', 'Enable mixing teams into battlegrounds for faster queues.\r\nDefault: 0 (Disabled)\r\n         1 (Enabled)');
INSERT INTO `config` VALUES ('1', 'DBC.Locale', '255', 'DBC Language Settings\n0 = English; 1 = Korean; 2 = French; 3 = German; 4 = Chinese; 5 = Taiwanese; 6 = Spanish; 7 = Spanish Mexico\n8 = Russian; 255 = Auto Detect (Default)');
INSERT INTO `config` VALUES ('1', 'Death.Bones.BattlegroundOrArena', '1', 'Enabled/disabled creating bones instead corpse at resurrection (in normal zones/instacnes, or battleground/arenas)\nDefault: 1 (enabled)\n         0 (disabled)');
INSERT INTO `config` VALUES ('1', 'Death.Bones.World', '1', 'Enabled/disabled creating bones instead corpse at resurrection (in normal zones/instacnes, or battleground/arenas)\nDefault: 1 (enabled)\n         0 (disabled)');
INSERT INTO `config` VALUES ('1', 'Death.CorpseReclaimDelay.PvE', '1', 'Enabled/disabled increase corpse reclaim delay at often PvP/PvE deaths\nDefault: 1 (enabled)\n         0 (disabled)');
INSERT INTO `config` VALUES ('1', 'Death.CorpseReclaimDelay.PvP', '1', 'Enabled/disabled increase corpse reclaim delay at often PvP/PvE deaths\nDefault: 1 (enabled)\n         0 (disabled)');
INSERT INTO `config` VALUES ('1', 'Death.Ghost.RunSpeed.Battleground', '1.000000', 'Modifies the speed of player\'s ghosts, removed upon reviving, not permanent/saved, in non-BG and BG maps\nDefault: 1.0 (normal speed)');
INSERT INTO `config` VALUES ('1', 'Death.Ghost.RunSpeed.World', '1.000000', 'Modifies the speed of player\'s ghosts, removed upon reviving, not permanent/saved, in non-BG and BG maps\nDefault: 1.0 (normal speed)');
INSERT INTO `config` VALUES ('1', 'Death.SicknessLevel', '11', 'Starting Character start gain sickness at spirit resurrection (1 min)\nDefault: 11\n         -10 - character will have full time (10min) sickness at 1 level\n         maxplayerlevel+1 - character will not have sickness at any level');
INSERT INTO `config` VALUES ('1', 'DeclinedNames', '0', 'Allow russian clients to set and use declined names\nDefault: 0 - do not use declined names, except when the Russian RealmZone is set\n         1 - use declined names');
INSERT INTO `config` VALUES ('1', 'DetectPosCollision', '1', 'Check final move position, summon position, etc for visible collision with other objects or\nwall (wall only if vmaps are enabled)\nDefault: 1 (enable, requires more CPU power)\n         0 (disable, not so nice position selection but will require less CPU po');
INSERT INTO `config` VALUES ('1', 'DurabilityLossChance.Absorb', '0.500000', 'Chance lost one from armor items durability point at damage absorb.\nDefault: 0.5 (100/0.5 = 200) Each 200 absorbs apply one from 15 possible armor equipped items');
INSERT INTO `config` VALUES ('1', 'DurabilityLossChance.Block', '0.050000', 'Chance lost sheild durability point at damage block.\nDefault: 0.05 (100/0.05 = 2000) Each 2000 partly or full blocked attacks shield lost point');
INSERT INTO `config` VALUES ('1', 'DurabilityLossChance.Damage', '0.500000', 'Chance lost one from equiped items durability point at damage apply or receive.\nDefault: 0.5 (100/0.5 = 200) Each 200 damage apply one from 19 possible equipped items');
INSERT INTO `config` VALUES ('1', 'DurabilityLossChance.Parry', '0.050000', 'Chance lost weapon durability point at parry.\nDefault: 0.05 (100/0.05 = 2000) Each 2000 parry attacks main weapon lost point');
INSERT INTO `config` VALUES ('1', 'Event.Announce', '0', 'Announce game events\r\nDefault: 0 (false)\r\n         1 (true)');
INSERT INTO `config` VALUES ('1', 'Expansion', '1', 'Allow server use content from expansion\n         2 - check expansion 2 maps existence, and if client support expansion 2 and account have\n             expansion 2 setting then allow visit expansion 2 maps, allow create new class character)\nDefault: 1 - ch');
INSERT INTO `config` VALUES ('1', 'GameType', '0', 'Server realm style\n0 = NORMAL; 1 = PVP; 4 = NORMAL; 6 = RP; 8 = RPPVP\nalso custom type: 16 FFA_PVP (free for all pvp mode like arena PvP in all zones except rest\nactivated places and sanctuaries)');
INSERT INTO `config` VALUES ('1', 'GM.AcceptTickets', '2', 'Is GM accepting tickets from player by default or not.\nDefault: 2 (last save state)\n         0 (disable)\n         1 (enable)');
INSERT INTO `config` VALUES ('1', 'GM.Chat', '2', 'GM chat mode at login\nDefault: 2 (last save state)\n         0 (disable)\n         1 (enable)');
INSERT INTO `config` VALUES ('1', 'GM.InGMList.Level', '3', 'Max GM level showed in GM list (if visible) in non-GM state (.gm off)\n         0 (none)\n         1 (only moderators)\n         2 (only gamemasters)\nDefault: 3 (anyone)');
INSERT INTO `config` VALUES ('1', 'GM.InvisibleAura', '37800', 'A spell that will be cast when a gm uses \".gm visible off\" (Must be aura spell)\nDefault: 37800 (50% Transparency)\n         0 (Disabled)');
INSERT INTO `config` VALUES ('1', 'GM.InWhoList.Level', '3', 'Max GM level showed in who list (if visible).\n         0 (only players)\n         1 (only moderators)\n         2 (only gamemasters)\nDefault: 3 (anyone)');
INSERT INTO `config` VALUES ('1', 'GM.LoginState', '2', 'GM mode at login\nDefault: 2 (last save state)\n         0 (disable)\n         1 (enable)');
INSERT INTO `config` VALUES ('1', 'GM.LogTrade', '0', 'Include GM trade and trade slot enchanting operations in GM log if it enable\nDefault: 1 (include)\n         0 (not include)');
INSERT INTO `config` VALUES ('1', 'GM.LowerSecurity', '0', 'Disallow a lower security member to interact with a higher one using commands\nDefault: 0 (disable)\n         1 (enable)');
INSERT INTO `config` VALUES ('1', 'GM.StartLevel', '1', 'GM starting level (1-255)\nDefault: 1');
INSERT INTO `config` VALUES ('1', 'GM.Visible', '2', 'GM visibility at login\nDefault: 2 (last save state)\n         0 (invisible)\n         1 (visible)');
INSERT INTO `config` VALUES ('1', 'GM.WhisperingTo', '2', 'Is GM accepting whispers from player by default or not.\nDefault: 2 (last save state)\n         0 (disable)\n         1 (enable)');
INSERT INTO `config` VALUES ('1', 'GridCleanUpDelay', '300000', 'Grid clean up delay (in milliseconds)\nDefault: 300000 (5 min)');
INSERT INTO `config` VALUES ('1', 'GridUnload', '1', 'Unload grids (if you have lot memory you can disable it to speed up player move to new grids second time)\nDefault: 1 (unload grids)\n         0 (do not unload grids)');
INSERT INTO `config` VALUES ('1', 'GuarderSight', '50.000000', 'Unknown, not in original mangosd.conf');
INSERT INTO `config` VALUES ('1', 'GuidReserveSize.Creature', '100', 'Amount guids reserved for .npc add/.gobject add directly after last used in DB static spawned creature/gameobject guid\nCommands .npc add/.gobject add can be used only for guids from this reserve and required server restart if all guids\nfrom reserve used b');
INSERT INTO `config` VALUES ('1', 'GuidReserveSize.GameObject', '100', 'Amount guids reserved for .npc add/.gobject add directly after last used in DB static spawned creature/gameobject guid\nCommands .npc add/.gobject add can be used only for guids from this reserve and required server restart if all guids\nfrom reserve used b');
INSERT INTO `config` VALUES ('1', 'Guild.BankEventLogRecordsCount', '25', 'Count of guild_bank event log records stored in guild_bank_eventlog table\nIncrease to store more guild_bank events in table - minimum is 25 (GUILD_BANK_MAX_LOGS) for each guild_bank tab\nUseful when you don\'t want old log events to be overwritten by new, b');
INSERT INTO `config` VALUES ('1', 'Guild.EventLogRecordsCount', '100', 'Count of guild event log records stored in guild_eventlog table\nIncrease to store more guild events in table, minimum is 100\nYou can set it to very high value to prevent oldest guild events to be rewritten by latest guild events - but it can slow down per');
INSERT INTO `config` VALUES ('1', 'Instance.IgnoreLevel', '0', 'Ignore level requirement to enter instance\nDefault: 0 (false)\n         1 (true)');
INSERT INTO `config` VALUES ('1', 'Instance.IgnoreRaid', '0', 'Ignore raid requirement to enter instance\nDefault: 0 (false)\n         1 (true)');
INSERT INTO `config` VALUES ('1', 'Instance.ResetTimeHour', '4', 'The hour of the day (0-23) when the global instance resets occur.\nDefault: 4');
INSERT INTO `config` VALUES ('1', 'Instance.UnloadDelay', '1800000', 'Unload the instance map from memory after some time if no players are inside.\nDefault: 1800000 (miliseconds, i.e 30 minutes)\n         0 (instance maps are kept in memory until they are reset)');
INSERT INTO `config` VALUES ('1', 'InstantLogout', '1', 'Enable or disable instant logout for security level (0..4) or high (NOT in combat/while dueling/while falling)\nDefault: 1 (Mods/GMs/Admins)');
INSERT INTO `config` VALUES ('1', 'ListenRange.Say', '40.000000', 'Distance from player to listen text that creature (or other world object) say\nDefault: 40');
INSERT INTO `config` VALUES ('1', 'ListenRange.TextEmote', '40.000000', 'Distance from player to listen textemote that creature (or other world object) say\nDefault: 40');
INSERT INTO `config` VALUES ('1', 'ListenRange.Yell', '300.000000', 'Distance from player to listen text that creature (or other world object) yell\nDefault: 300');
INSERT INTO `config` VALUES ('1', 'MailDeliveryDelay', '3600', 'Mail delivery delay time for item sending\nDefault: 3600 sec (1 hour)');
INSERT INTO `config` VALUES ('1', 'MapUpdateInterval', '100', 'Map update interval (in milliseconds)\nDefault: 100');
INSERT INTO `config` VALUES ('1', 'MassMailer.SendPerTick', '10', 'Max amount mail send each tick from mails list scheduled for mass mailer proccesing.\nMore mails increase server load but speedup mass mail proccess. Normal tick length: 50 msecs, so 20 ticks in sec and 200 mails in sec by default.\nDefault: 10');
INSERT INTO `config` VALUES ('1', 'MaxArenaPoints', '5000', 'Max arena points that player can have.\nDefault: 5000');
INSERT INTO `config` VALUES ('1', 'MaxCoreStuckTime', '0', 'Periodically check if the process got freezed, if this is the case force crash after the specified\namount of seconds. Must be > 0. Recommended > 10 secs if you use this.\nDefault: 0 (Disabled)');
INSERT INTO `config` VALUES ('1', 'MaxGroupXPDistance', '74.000000', 'Max distance to creature for group memeber to get XP at creature death.\nDefault: 74');
INSERT INTO `config` VALUES ('1', 'MaxHonorPoints', '75000', 'Max honor points that player can have.\nDefault: 75000');
INSERT INTO `config` VALUES ('1', 'MaxOverspeedPings', '2', 'Maximum overspeed ping count before player kick (minimum is 2, 0 used to disable check)\nDefault: 2');
INSERT INTO `config` VALUES ('1', 'MaxPlayerLevel', '70', 'Max level that can be reached by a player for experience (in range from 1 to 100).\nChange not recommended\nDefault: 70');
INSERT INTO `config` VALUES ('1', 'MaxPrimaryTradeSkill', '2', 'Max count that player can learn the primary trade skill.\nDefault: 2\nMax : 10');
INSERT INTO `config` VALUES ('1', 'MaxSpellCastsInChain', '20', 'Max amount triggered spell casts in chain by one caster, prevent stack overflow crash\nToo Low value will make some correct triggered casts fail\n         0 (no limit)\nDefault: 20');
INSERT INTO `config` VALUES ('1', 'MinCharterName', '2', 'Minimal name length (1..24)\nDefault: 2');
INSERT INTO `config` VALUES ('1', 'MinPetitionSigns', '9', 'Min signatures count to creating guild (0..9).\nDefault: 9');
INSERT INTO `config` VALUES ('1', 'MinPetName', '2', 'Minimal name length (1..12)\nDefault: 2');
INSERT INTO `config` VALUES ('1', 'MinPlayerName', '2', 'Minimal name length (1..12)\nDefault: 2');
INSERT INTO `config` VALUES ('1', 'mmap.enabled', '1', 'Enable/Disable pathfinding using mmaps\nDefault: 1 (enable)\n         0 (disable)');
INSERT INTO `config` VALUES ('1', 'mmap.ignoreMapIds', '', 'Disable mmap pathfinding on the listed maps.\nList of map ids with delimiter \',\'');
INSERT INTO `config` VALUES ('1', 'MonsterSight', '50.000000', 'Unknown, not in original mangosd.conf');
INSERT INTO `config` VALUES ('1', 'Motd', 'Welcome to the Massive Network Game Object Server.', 'Message of the Day. Displayed at worldlogin for every user (\'@\' for a newline).');
INSERT INTO `config` VALUES ('1', 'Network.KickOnBadPacket', '0', 'Kick player on bad packet format.\nDefault: 0 - do not kick\n         1 - kick');
INSERT INTO `config` VALUES ('1', 'Network.OutKBuff', '-1', 'The size of the output kernel buffer used ( SO_SNDBUF socket option, tcp manual ).\nDefault: -1 (Use system default setting)');
INSERT INTO `config` VALUES ('1', 'Network.OutUBuff', '65536', 'Userspace buffer for output. This is amount of memory reserved per each connection.\nDefault: 65536');
INSERT INTO `config` VALUES ('1', 'Network.TcpNodelay', '1', 'TCP Nagle algorithm setting\nDefault: 0 (enable Nagle algorithm, less traffic, more latency)\n         1 (TCP_NO_DELAY, disable Nagle algorithm, more traffic but less latency)');
INSERT INTO `config` VALUES ('1', 'Network.Threads', '1', 'Number of threads for network, recommend 1 thread per 1000 connections.\nDefault: 1');
INSERT INTO `config` VALUES ('1', 'OffhandCheckAtTalentsReset', '0', 'Talent reset can change offhand weapon restrictions for equip slots.\nDefault: 0 - recheck offhand slot weapon only at zone update\n         1 - recheck offhand slot weapon at talent reset also');
INSERT INTO `config` VALUES ('1', 'OutdoorPvp.EPEnabled', '1', 'Enable Eastern Plaguelands Outdoor pvp');
INSERT INTO `config` VALUES ('1', 'OutdoorPvp.HPEnabled', '1', 'Enable Hellfire Peninsula Outdoor pvp');
INSERT INTO `config` VALUES ('1', 'OutdoorPvp.NAEnabled', '1', 'Enable Halaa (Nagrand) Outdoor pvp');
INSERT INTO `config` VALUES ('1', 'OutdoorPvp.SIEnabled', '1', 'Enable Silithus Outdoor pvp');
INSERT INTO `config` VALUES ('1', 'OutdoorPvp.TFEnabled', '1', 'Enable Terokkar Forest  Outdoor pvp');
INSERT INTO `config` VALUES ('1', 'OutdoorPvp.ZMEnabled', '1', 'Enable Zangarmarsh Outdoor pvp');
INSERT INTO `config` VALUES ('1', 'PetUnsummonAtMount', '1', 'Persmanent pet will unsummoned at player mount\n         0 - unsummon only for flying mounts\nDefault: 1 - unsummon for any mount');
INSERT INTO `config` VALUES ('1', 'PlayerCommands', '1', 'Should player chat be parsed for GM commands.\nDefault: 1 (parse commands)\n         0 (ignore commands)');
INSERT INTO `config` VALUES ('1', 'PlayerLimit', '100', 'Maximum number of players in the world. Excluding Mods, GM\'s and Admins\nDefault: 100\n         0 (for infinite players)\n        -1 (for Mods, GM\'s and Admins only)\n        -2 (for GM\'s and Admins only)\n        -3 (for Admins only)');
INSERT INTO `config` VALUES ('1', 'PlayerSave.Interval', '0', 'Player save interval (in milliseconds)\nDefault: 900000 (15 min)');
INSERT INTO `config` VALUES ('1', 'PlayerSave.PerSecond', '2', 'Player saves per second (only applied if PlayerSave.Interval = 0)\nDefault: 2 (0.5s between saves)');
INSERT INTO `config` VALUES ('1', 'PlayerSave.Stats.MinLevel', '0', 'Minimum level for saving character stats for external usage in database\nDefault: 0  (do not save character stats)\n         1+ (save stats for characters with level 1+)');
INSERT INTO `config` VALUES ('1', 'PlayerSave.Stats.SaveOnlyOnLogout', '1', 'Enable/Disable saving of character stats only on logout\nDefault: 1 (only save on logout)\n         0 (save on every player save)');
INSERT INTO `config` VALUES ('1', 'ProcessPriority', '0', 'Process priority setting (Used only at Windows)\nDefault: 1 (HIGH)\n         0 (Normal)');
INSERT INTO `config` VALUES ('1', 'Quests.Daily.ResetHour', '6', 'Hour when daily quests reset (0..23)\nDefault: 6');
INSERT INTO `config` VALUES ('1', 'Quests.HighLevelHideDiff', '7', 'Quest level difference to hide for player high level quests:\nif player_level < quest_min_level - HighLevelQuestsHideDiff then quest \"!\" mark not show for quest giver\nDefault: 7\n        -1 (show all available quests marks)');
INSERT INTO `config` VALUES ('1', 'Quests.IgnoreRaid', '0', 'Non-raid quests allowed in raids\nDefault: 0 (not allowed)\n         1 (allowed)');
INSERT INTO `config` VALUES ('1', 'Quests.LowLevelHideDiff', '4', 'Quest level difference to hide for player low level quests:\nif player_level > quest_level + LowLevelQuestsHideDiff then quest \"!\" mark not show for quest giver\nDefault: 4\n        -1 (show all available quests marks)');
INSERT INTO `config` VALUES ('1', 'Ra.Enable', '0', 'Enable remote console\nDefault: 0 - off\n         1 - on');
INSERT INTO `config` VALUES ('1', 'RabbitDay', '0', 'Set to Rabbit Day (date in unix time), only the day and month are considered, the year is not important\nDefault: 0 (off)\nSuggested: 954547200 (April 1st, 2000)');
INSERT INTO `config` VALUES ('1', 'Rate.Auction.Cut', '1.000000', 'Auction rates (auction time, deposit get at auction start, auction cut from price at auction end)');
INSERT INTO `config` VALUES ('1', 'Rate.Auction.Deposit', '1.000000', 'Auction rates (auction time, deposit get at auction start, auction cut from price at auction end)');
INSERT INTO `config` VALUES ('1', 'Rate.Auction.Time', '1.000000', 'Auction rates (auction time, deposit get at auction start, auction cut from price at auction end)');
INSERT INTO `config` VALUES ('1', 'Rate.Corpse.Decay.Looted', '0.000000', 'Controls how long the creature corpse stays after it had been looted, as a multiplier of its Corpse.Decay.* config.\nDefault: 0.0 (will use modifier /3 of the creatures spawntimesecs when 0.0)');
INSERT INTO `config` VALUES ('1', 'Rate.Creature.Aggro', '1.000000', 'Aggro radius percent or off.\nDefault: 1   - 100%\n         1.5 - 150%\n         0   - off (0%)');
INSERT INTO `config` VALUES ('1', 'Rate.Creature.Elite.Elite.Damage', '1.000000', 'Creature Damage Rates.\nExamples: 2 - creatures will damage 2x, 1.7 - 1.7x.');
INSERT INTO `config` VALUES ('1', 'Rate.Creature.Elite.Elite.HP', '1.000000', 'Creature Health Ammount Modifier.\nExamples: 2 - creatures have 2x health, 1.7 - 1.7x.');
INSERT INTO `config` VALUES ('1', 'Rate.Creature.Elite.Elite.SpellDamage', '1.000000', 'Creature Spell Damage Rates.\nExamples: 2 - creatures will damage with spells 2x, 1.7 - 1.7x.');
INSERT INTO `config` VALUES ('1', 'Rate.Creature.Elite.RARE.Damage', '1.000000', 'Creature Damage Rates.\nExamples: 2 - creatures will damage 2x, 1.7 - 1.7x.');
INSERT INTO `config` VALUES ('1', 'Rate.Creature.Elite.RARE.HP', '1.000000', 'Creature Health Ammount Modifier.\nExamples: 2 - creatures have 2x health, 1.7 - 1.7x.');
INSERT INTO `config` VALUES ('1', 'Rate.Creature.Elite.RARE.SpellDamage', '1.000000', 'Creature Spell Damage Rates.\nExamples: 2 - creatures will damage with spells 2x, 1.7 - 1.7x.');
INSERT INTO `config` VALUES ('1', 'Rate.Creature.Elite.RAREELITE.Damage', '1.000000', 'Creature Damage Rates.\nExamples: 2 - creatures will damage 2x, 1.7 - 1.7x.');
INSERT INTO `config` VALUES ('1', 'Rate.Creature.Elite.RAREELITE.HP', '1.000000', 'Creature Health Ammount Modifier.\nExamples: 2 - creatures have 2x health, 1.7 - 1.7x.');
INSERT INTO `config` VALUES ('1', 'Rate.Creature.Elite.RAREELITE.SpellDamage', '1.000000', 'Creature Spell Damage Rates.\nExamples: 2 - creatures will damage with spells 2x, 1.7 - 1.7x.');
INSERT INTO `config` VALUES ('1', 'Rate.Creature.Elite.WORLDBOSS.Damage', '1.000000', 'Creature Damage Rates.\nExamples: 2 - creatures will damage 2x, 1.7 - 1.7x.');
INSERT INTO `config` VALUES ('1', 'Rate.Creature.Elite.WORLDBOSS.HP', '1.000000', 'Creature Health Ammount Modifier.\nExamples: 2 - creatures have 2x health, 1.7 - 1.7x.');
INSERT INTO `config` VALUES ('1', 'Rate.Creature.Elite.WORLDBOSS.SpellDamage', '1.000000', 'Creature Spell Damage Rates.\nExamples: 2 - creatures will damage with spells 2x, 1.7 - 1.7x.');
INSERT INTO `config` VALUES ('1', 'Rate.Creature.Normal.Damage', '1.000000', 'Creature Damage Rates.\nExamples: 2 - creatures will damage 2x, 1.7 - 1.7x.');
INSERT INTO `config` VALUES ('1', 'Rate.Creature.Normal.HP', '1.000000', 'Creature Health Ammount Modifier.\nExamples: 2 - creatures have 2x health, 1.7 - 1.7x.');
INSERT INTO `config` VALUES ('1', 'Rate.Creature.Normal.SpellDamage', '1.000000', 'Creature Spell Damage Rates.\nExamples: 2 - creatures will damage with spells 2x, 1.7 - 1.7x.');
INSERT INTO `config` VALUES ('1', 'Rate.Damage.Fall', '1.000000', 'Damage after fall rate. (1 - standard, 2 - double damage, 0.5 - half damage, etc)');
INSERT INTO `config` VALUES ('1', 'Rate.Drop.Item.Artifact', '1.000000', 'Drop rates (items by quality and money)\nDefault: 1');
INSERT INTO `config` VALUES ('1', 'Rate.Drop.Item.Epic', '1.000000', 'Drop rates (items by quality and money)\nDefault: 1');
INSERT INTO `config` VALUES ('1', 'Rate.Drop.Item.Legendary', '1.000000', 'Drop rates (items by quality and money)\nDefault: 1');
INSERT INTO `config` VALUES ('1', 'Rate.Drop.Item.Normal', '1.000000', 'Drop rates (items by quality and money)\nDefault: 1');
INSERT INTO `config` VALUES ('1', 'Rate.Drop.Item.Poor', '1.000000', 'Drop rates (items by quality and money)\nDefault: 1');
INSERT INTO `config` VALUES ('1', 'Rate.Drop.Item.Rare', '1.000000', 'Drop rates (items by quality and money)\nDefault: 1');
INSERT INTO `config` VALUES ('1', 'Rate.Drop.Item.Referenced', '1.000000', 'Drop rates (items by quality and money)\nDefault: 1');
INSERT INTO `config` VALUES ('1', 'Rate.Drop.Item.Uncommon', '1.000000', 'Drop rates (items by quality and money)\nDefault: 1');
INSERT INTO `config` VALUES ('1', 'Rate.Drop.Money', '1.000000', 'Drop rates (items by quality and money)\nDefault: 1');
INSERT INTO `config` VALUES ('1', 'Rate.Energy', '1.000000', 'Health and power regeneration and rage income from damage.\nDefault: 1');
INSERT INTO `config` VALUES ('1', 'Rate.Focus', '1.000000', 'Health and power regeneration and rage income from damage.\nDefault: 1');
INSERT INTO `config` VALUES ('1', 'Rate.Health', '1.000000', 'Health and power regeneration and rage income from damage.\nDefault: 1');
INSERT INTO `config` VALUES ('1', 'Rate.Honor', '1.000000', 'Honor gain rate');
INSERT INTO `config` VALUES ('1', 'Rate.InstanceResetTime', '1.000000', 'Multiplier for the number of days in between global raid/heroic instance resets.\nDefault: 1');
INSERT INTO `config` VALUES ('1', 'Rate.Loyalty', '1.000000', 'Health and power regeneration and rage income from damage.\nDefault: 1');
INSERT INTO `config` VALUES ('1', 'Rate.Mana', '1.000000', 'Health and power regeneration and rage income from damage.\nDefault: 1');
INSERT INTO `config` VALUES ('1', 'Rate.Mining.Amount', '1.000000', 'Mining Rates (Mining.Amount changes minimum/maximum usetimes of a deposit,\nMining.Next changes chance to have next use of a deposit)');
INSERT INTO `config` VALUES ('1', 'Rate.Mining.Next', '1.000000', 'Mining Rates (Mining.Amount changes minimum/maximum usetimes of a deposit,\nMining.Next changes chance to have next use of a deposit)');
INSERT INTO `config` VALUES ('1', 'Rate.Rage.Income', '1.000000', 'Health and power regeneration and rage income from damage.\nDefault: 1');
INSERT INTO `config` VALUES ('1', 'Rate.Rage.Loss', '1.000000', 'Health and power regeneration and rage income from damage.\nDefault: 1');
INSERT INTO `config` VALUES ('1', 'Rate.Reputation.Gain', '1.000000', 'Reputation Gain rate\nDefault: 1');
INSERT INTO `config` VALUES ('1', 'Rate.Reputation.LowLevel.Kill', '1.000000', 'Reputation Gain form low level kill (grey creture)\nDefault: 0.2');
INSERT INTO `config` VALUES ('1', 'Rate.Reputation.LowLevel.Quest', '1.000000', 'Reputation Gain rate\nDefault: 1');
INSERT INTO `config` VALUES ('1', 'Rate.Rest.InGame', '1.000000', 'Resting points grow rates (1 - normal, 2 - double rate, 0.5 - half rate, etc) from standard values');
INSERT INTO `config` VALUES ('1', 'Rate.Rest.Offline.InTavernOrCity', '1.000000', 'Resting points grow rates (1 - normal, 2 - double rate, 0.5 - half rate, etc) from standard values');
INSERT INTO `config` VALUES ('1', 'Rate.Rest.Offline.InWilderness', '1.000000', 'Resting points grow rates (1 - normal, 2 - double rate, 0.5 - half rate, etc) from standard values');
INSERT INTO `config` VALUES ('1', 'Rate.Skill.Discovery', '1.000000', 'Skill Discovery Rates\nDefault: 1');
INSERT INTO `config` VALUES ('1', 'Rate.Talent', '1.000000', 'Talent Point rates\nDefault: 1');
INSERT INTO `config` VALUES ('1', 'Rate.XP.Explore', '1.000000', 'XP rates\nDefault: 1');
INSERT INTO `config` VALUES ('1', 'Rate.XP.Kill', '1.000000', 'XP rates\nDefault: 1');
INSERT INTO `config` VALUES ('1', 'Rate.XP.Quest', '1.000000', 'XP rates\nDefault: 1');
INSERT INTO `config` VALUES ('1', 'RealmZone', '1', 'Server realm zone (set allowed alphabet in character names/etc). See also Strict*Names options.\n\n    1 Development    - any language (Default)\n    2 United States  - extended-Latin\n    3 Oceanic        - extended-Latin\n    4 Latin America  - extended-Lati');
INSERT INTO `config` VALUES ('1', 'SaveRespawnTimeImmediately', '1', 'Save respawn time for creatures at death and for gameobjects at use/open\nDefault: 1 (save creature/gameobject respawn time without waiting grid unload)\n         0 (save creature/gameobject respawn time at grid unload)');
INSERT INTO `config` VALUES ('1', 'SkillChance.Green', '25', 'Skill chance values (0..100)\nDefault: 100-75-25-0');
INSERT INTO `config` VALUES ('1', 'SkillChance.Grey', '0', 'Skill chance values (0..100)\nDefault: 100-75-25-0');
INSERT INTO `config` VALUES ('1', 'SkillChance.MiningSteps', '75', 'For skinning and Mining chance decrease with skill level.\nDefault: 0  - no decrease\n         75 - in 2 times each 75 skill points');
INSERT INTO `config` VALUES ('1', 'SkillChance.Orange', '100', 'Skill chance values (0..100)\nDefault: 100-75-25-0');
INSERT INTO `config` VALUES ('1', 'SkillChance.Prospecting', '0', 'For prospecting skillup not possible by default, but can be allowed as custom setting\nDefault: 0 - no skilups\n         1 - skilups possible');
INSERT INTO `config` VALUES ('1', 'SkillChance.SkinningSteps', '75', 'For skinning and Mining chance decrease with skill level.\nDefault: 0  - no decrease\n         75 - in 2 times each 75 skill points');
INSERT INTO `config` VALUES ('1', 'SkillChance.Yellow', '75', 'Skill chance values (0..100)\nDefault: 100-75-25-0');
INSERT INTO `config` VALUES ('1', 'SkillFail.Gain.Fishing', '0', 'For fishing skill gain possible at fail also\nDefault: 0 (disabled)\n         1 (enabled)');
INSERT INTO `config` VALUES ('1', 'SkillFail.Loot.Fishing', '0', 'For fishing instead fail provided junk loot\nDefault: 0 (disabled)\n         1 (enabled)');
INSERT INTO `config` VALUES ('1', 'SkillFail.Possible.FishingPool', '1', 'For fishing pool impossible fail from low skill by default\nDefault: 1 (enabled)\n         0 (disabled)');
INSERT INTO `config` VALUES ('1', 'SkillGain.Crafting', '1', 'crafting/defense/gathering/weapon skills gain at skill grow (1,2,...)\nDefault: 1');
INSERT INTO `config` VALUES ('1', 'SkillGain.Defense', '1', 'crafting/defense/gathering/weapon skills gain at skill grow (1,2,...)\nDefault: 1');
INSERT INTO `config` VALUES ('1', 'SkillGain.Gathering', '1', 'crafting/defense/gathering/weapon skills gain at skill grow (1,2,...)\nDefault: 1');
INSERT INTO `config` VALUES ('1', 'SkillGain.Weapon', '1', 'crafting/defense/gathering/weapon skills gain at skill grow (1,2,...)\nDefault: 1');
INSERT INTO `config` VALUES ('1', 'SkipCinematics', '0', 'Disable in-game script movie at first character\'s login(allows to prevent buggy intro in case of custom start location coordinates)\nDefault: 0 - show intro for each new character\n         1 - show intro only for first character of selected race\n         2');
INSERT INTO `config` VALUES ('1', 'SOAP.Enabled', '0', 'Enable soap service\nDefault: 0 - off\n         1 - on');
INSERT INTO `config` VALUES ('1', 'SOAP.IP', '127.0.0.1', 'Bound SOAP service ip address, use 0.0.0.0 to access from everywhere\r\nDefault: 127.0.0.1');
INSERT INTO `config` VALUES ('1', 'SOAP.Port', '7878', 'SOAP port\r\nDefault: 7878');
INSERT INTO `config` VALUES ('1', 'StartArenaPoints', '0', 'Amount of arena points that new players will start with\nDefault: 0');
INSERT INTO `config` VALUES ('1', 'StartHonorPoints', '0', 'Amount of honor that new players will start with\nDefault: 0');
INSERT INTO `config` VALUES ('1', 'StartPlayerLevel', '1', 'Staring level that have character at creating (in range 1 to MaxPlayerLevel)\nDefault: 1');
INSERT INTO `config` VALUES ('1', 'StartPlayerMoney', '0', 'Amount of money that new players will start with.\nIf you want to start with one silver, use for example 100 (100 copper = 1 silver)\nDefault: 0');
INSERT INTO `config` VALUES ('1', 'StrictCharterNames', '0', 'Limit guild/arena team charter names to language specific symbols set, not allow create charters with not allowed symbols in name\nDefault: 0 disable\n         1 basic latin characters  (strict)\n         2 realm zone specific (strict). See RealmZone setting');
INSERT INTO `config` VALUES ('1', 'StrictPetNames', '0', 'Limit pet names to language specific symbols set\nDefault: 0 disable\n         1 basic latin characters  (strict)\n         2 realm zone specific (strict). See RealmZone setting.\n           Note: In any case if you want correctly see character name at client');
INSERT INTO `config` VALUES ('1', 'StrictPlayerNames', '0', 'Limit player name to language specific symbols set, not allow create characters, and set rename request and disconnect at not allowed symbols name\nDefault: 0 disable (but limited server timezone dependent client check)\n         1 basic latin characters  (');
INSERT INTO `config` VALUES ('1', 'TalentsInspecting', '1', 'Allow other players see character talents in inspect dialog (Characters in Gamemaster mode can\ninspect talents always)\nDefault: 1 (allow)\n         0 (not allow)');
INSERT INTO `config` VALUES ('1', 'TargetPosRecalculateRange', '1.500000', 'Max distance from movement target point (+moving unit size) and targeted object (+size)\nafter that new target movement point calculated. Max: melee attack range (5), min: contact range (0.5)\nMore distance let have better performence, less distance let hav');
INSERT INTO `config` VALUES ('1', 'ThreatRadius', '100.000000', 'Radius for creature to evade after being pulled away from combat start point\nIf ThreatRadius is less than creature aggro radius then aggro radius will be used\nDefault: 100 yards');
INSERT INTO `config` VALUES ('1', 'TimerBar.Breath.GMLevel', '4', 'Disable/enable waterbreathing for security level (0..4) or high\nDefault: 4 (None)');
INSERT INTO `config` VALUES ('1', 'TimerBar.Breath.Max', '180', 'Disable/enable waterbreathing for security level (0..4) or high\nDefault: 4 (None)');
INSERT INTO `config` VALUES ('1', 'TimerBar.Fatigue.GMLevel', '4', 'Disable/enable fatigue for security level (0..4) or high\nDefault: 4 (None)');
INSERT INTO `config` VALUES ('1', 'TimerBar.Fatigue.Max', '60', 'Fatigue max timer value (in secs)\nDefault: 60 (1 minute)\n          0 (instant death)');
INSERT INTO `config` VALUES ('1', 'TimerBar.Fire.GMLevel', '4', 'Disable/enable lava fire damage for security level (0..4) or high\nDefault: 4 (None)');
INSERT INTO `config` VALUES ('1', 'TimerBar.Fire.Max', '1', 'Lava damage delay max timer value (in secs)\nDefault:  1\n          0 (instant in lava damage start)');
INSERT INTO `config` VALUES ('1', 'TradeSkill.GMIgnore.Level', '4', 'GM level starting from trade skill level requirement ignored.\nDefault: 4 (Console as noneone)');
INSERT INTO `config` VALUES ('1', 'TradeSkill.GMIgnore.MaxPrimarySkillsCount', '4', 'GM level starting from max primary skill count requirement ignored.\nDefault: 4 (Console as noneone)');
INSERT INTO `config` VALUES ('1', 'TradeSkill.GMIgnore.Skill', '4', 'GM level starting from trade skill skill requirement ignored.\nDefault: 4 (Console as noneone)');
INSERT INTO `config` VALUES ('1', 'UpdateUptimeInterval', '10', 'Update realm uptime period in minutes (for save data in \'uptime\' table). Must be > 0\nDefault: 10 (minutes)');
INSERT INTO `config` VALUES ('1', 'Visibility.AIRelocationNotifyDelay', '1000', 'Delay time between creature AI reactions on nearby movements\nDefault: 1000 (milliseconds)');
INSERT INTO `config` VALUES ('1', 'Visibility.Distance.BGArenas', '180.000000', 'Visibility distance for different ingame object in different maps.\nVisibility on continents on offy ~90 yards. In BG/Arenas ~180. For instances default ~120.\nMax limited by active player zone: ~ 333\nMin limit is max aggro radius (45) * Rate.Creature.Aggro');
INSERT INTO `config` VALUES ('1', 'Visibility.Distance.Continents', '90.000000', 'Visibility distance for different ingame object in different maps.\nVisibility on continents on offy ~90 yards. In BG/Arenas ~180. For instances default ~120.\nMax limited by active player zone: ~ 333\nMin limit is max aggro radius (45) * Rate.Creature.Aggro');
INSERT INTO `config` VALUES ('1', 'Visibility.Distance.Grey.Object', '10.000000', 'Visibility grey distance for dynobjects/gameobjects/corpses/creature bodies\nDefault: 10 (yards)');
INSERT INTO `config` VALUES ('1', 'Visibility.Distance.Grey.Unit', '1.000000', 'Visibility grey distance for creatures/players (fast changing objects)\naddition to appropriate object type Visibility.Distance.* use in case visibility removing to\nobject (except corpse around distances) If D is distance and G is grey distance then object');
INSERT INTO `config` VALUES ('1', 'Visibility.Distance.InFlight', '90.000000', 'Visible distance for player in flight\nMin limit is 0 (not show any objects)');
INSERT INTO `config` VALUES ('1', 'Visibility.Distance.Instances', '120.000000', 'Visibility distance for different ingame object in different maps.\nVisibility on continents on offy ~90 yards. In BG/Arenas ~180. For instances default ~120.\nMax limited by active player zone: ~ 333\nMin limit is max aggro radius (45) * Rate.Creature.Aggro');
INSERT INTO `config` VALUES ('1', 'Visibility.GroupMode', '0', 'Group visibility modes\nDefault: 0 (standard setting: only members from same group can 100% auto detect invisible player)\n         1 (raid members 100% auto detect invisible player from same raid)\n         2 (players from same team can 100% auto detect inv');
INSERT INTO `config` VALUES ('1', 'Visibility.RelocationLowerLimit', '10.000000', 'Object\'s visibility update called, when distance between current object\'s position and position,\nwhere visiblity was updated last time, reaches RelocationLoverLimit value\nDefault: 10 (yards)');
INSERT INTO `config` VALUES ('1', 'vmap.enableHeight', '1', 'Enable/Disable VMaps support for line of sight and height calculation\nYou need to extract the VMaps in order to enable those options\nDefault: 1 (enable)\n         0 (disable)');
INSERT INTO `config` VALUES ('1', 'vmap.enableIndoorCheck', '1', 'Enable/Disable VMap based indoor check to remove outdoor-only auras (mounts etc.).\nRequires VMaps enabled to work.\nDefault: 1 (Enabled)\n         0 (Disabled)');
INSERT INTO `config` VALUES ('1', 'vmap.enableLOS', '0', 'Enable/Disable VMaps support for line of sight and height calculation\nYou need to extract the VMaps in order to enable those options\nDefault: 1 (enable)\n         0 (disable)');
INSERT INTO `config` VALUES ('1', 'vmap.ignoreSpellIds', '', 'These spells are ignored for LoS calculation\nList of ids with delimiter \',\'');
INSERT INTO `config` VALUES ('1', 'Warden.BanLength', '900000', 'Undocumented');
INSERT INTO `config` VALUES ('1', 'Warden.ClientCheckHoldOff', '30', 'Undocumented');
INSERT INTO `config` VALUES ('1', 'Warden.ClientResponseDelay', '15', 'Undocumented');
INSERT INTO `config` VALUES ('1', 'Warden.Enabled', '0', 'Undocumented');
INSERT INTO `config` VALUES ('1', 'Warden.Kick.Ban', '0.000000', 'Undocumented');
INSERT INTO `config` VALUES ('1', 'Warden.NumChecks', '3', 'Undocumented');
INSERT INTO `config` VALUES ('1', 'WorldBossLevelDiff', '3', 'Difference for boss dynamic level with target\nDefault: 3');
