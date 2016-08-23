# nand-injector
Inject bytes into NAND architecture ( Page + OOB )

Main idea is to provide ECC for affected pages by insertion, but as too many algorithms provided ( and non-provided ) by flash controllers, this feature remain unavailable, but only FF write to OBB to make sure no bad block we made for future NAND programs. There's a possibility for listing all known NAND flashes with known ECC algorithms, I look through it in near future. If you have any experience in ECC calculations, I would be happy if help me to complete this little tiny tool.

For now, this mini tool helped me a lot!