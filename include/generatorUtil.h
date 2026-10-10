#ifndef GENERATOR_UTIL_H
#define GENERATOR_UTIL_H
typedef struct {
    long long user;
    long long edge;
    long long hashTag;
    long long post;
    long long check;
} GeneratorConfig;

int LoadGeneratorConfig(GeneratorConfig* config);
long long RandomID(long long range);
#endif