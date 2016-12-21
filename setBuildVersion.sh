#!/bin/bash
sed -i '.bak' "s/.*VERSION_COMMIT_COUNT.*/#define VERSION_COMMIT_COUNT \"$(git rev-list --count master)\"/" tsschecker/all_tsschecker.h 
sed -i '.bak' "s/.*VERSION_COMMIT_SHA.*/#define VERSION_COMMIT_SHA \"$(git rev-parse HEAD)\"/" tsschecker/all_tsschecker.h 

