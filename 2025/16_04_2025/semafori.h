union semun
{
  int val;
  struct semid_ds* buf;
  unsigned short * array;
};

int SEM_SET(int sem_id, int sem_val)
{
  union semun sem_union;
  sem_union.val = sem_val;
  return semctl(sem_id, 0, SETVAL, sem_union);
}

int SEM_DEL(int sem_id)
{
  return semctl(sem_id, 0, IPC_RMID);
}

int SEM_P(int sem_id)
{
  struct sembuf sem_b;
  sem_b.sem_num = 0;
  sem_b.sem_op = -1;
  sem_b.sem_flg = SEM_UNDO;
  return semop(sem_id, &sem_b, 1);
}

int SEM_V(int sem_id)
{
  struct sembuf sem_b;
  sem_b.sem_num = 0;
  sem_b.sem_op = 1;
  sem_b.sem_flg = SEM_UNDO;
  return semop(sem_id, &sem_b, 1);
}


