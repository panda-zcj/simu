/**
 * @author: pikaqiu_zcj
 * @date: 2024-07-21
 * @LastEditors: pikaqiu_zcj
 * @LastEditTime: 2024-07-21
 * @file: \src\generatesimulationscene\generatesimulationscene.cpp
 * @message:
 * @Copyright (c) 2024 by pikaqiu, All Rights Reserved.
 */

#include "generatesimulationscene.h"
#include "log.h"
#include <vector>
using namespace simu;

GenerateSimulationScene::GenerateSimulationScene(/* args */)
{
}

GenerateSimulationScene::~GenerateSimulationScene()
{
}

bool GenerateSimulationScene::operator()(nlohmann::ordered_json &simulator)
{
    std::vector<std::string> active;
    int totalNodeNums = 0;
    int onlineNodeNums = 0;
    for (auto it = simulator["nodes"]["detail"].begin(); it != simulator["nodes"]["detail"].end(); it++)
    {
        active.emplace_back(it.key());
        totalNodeNums++;
        onlineNodeNums++;
    }
    simulator["nodes"]["active"] = active;
    simulator["nodes"]["totalNodeNums"] = totalNodeNums;
    simulator["nodes"]["onlineNodeNums"] = onlineNodeNums;
    return true;
}