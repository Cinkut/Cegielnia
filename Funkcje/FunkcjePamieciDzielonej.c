#include "FunkcjePamieciDzielonej.h"

// Tworzy segment pamięci dzielonej
int create_shared_memory(const char *path, int identifier, size_t memory_amount, int flags)
{
	key_t key = ftok(path, identifier); // Generujemy unikalny klucz
	if ( key == - 1)
    {
        perror("Ftok failed");
        exit(1);
    }

	int shm_id = shmget(key, memory_amount, flags); // Tworzymy/otwieramy segment
	if (shm_id == -1)
    {
        perror("Shmget failed");
        exit(2);
    }

	return shm_id;
}

// Podłącza segment pamięci dzielonej do przestrzeni adresowej procesu
void* attach_shared_memory(int shm_id, const void* shm_adress, int flags)
{	
	void* shared_memory_adress = shmat(shm_id, shm_adress, flags);
	if (shared_memory_adress == (void*)-1)
	{
		perror("Shmat failed");
		free_shared_memory(shm_id);
		exit(2);
	}
	return shared_memory_adress;
}

// Odłącza segment pamięci dzielonej od przestrzeni adresowej procesu
void detach_shared_memory(const void* shm_adress, int shm_id)
{
	int result = shmdt(shm_adress);
	if ( result == -1 )
	{
		perror("Shmdt failed");
		free_shared_memory(shm_id);
		exit(3);
	}
}

// Usuwa segment pamięci dzielonej
void free_shared_memory(int shm_id)
{
	struct shmid_ds buf;
	if (shmctl(shm_id, IPC_STAT, &buf) == -1 ) 
	{ 
		return; 
	}

	if (shmctl(shm_id, IPC_RMID, NULL) == -1 )
	{
		perror("Shmctl IPC_RMID error");
		exit(4);
	}
}