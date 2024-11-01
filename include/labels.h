#ifndef LABELS_H
#define LABELS_H

struct label
{
    char name[DEF_SIZE];
    int ip;
};

struct file
{
    char name[DEF_SIZE];
    int ip;
};

static label Labels[DEF_SIZE] = {};
static file Files[DEF_SIZE] = {};

#endif
