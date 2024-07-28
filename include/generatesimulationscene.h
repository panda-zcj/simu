/**
 * @author: pikaqiu_zcj
 * @date: 2024-07-21
 * @LastEditors: pikaqiu_zcj
 * @LastEditTime: 2024-07-21
 * @file: \include\generatesimulationscene.h
 * @message:
 * @Copyright (c) 2024 by pikaqiu, All Rights Reserved.
 */
#ifndef GENERATE_SIMULATION_SCENE_H
#define GENERATE_SIMULATION_SCENE_H

#include "global.h"
#include "nlohmann/json.hpp"

namespace simu
{
    /**
     * @brief generating simulation scene for the simulator running
     */
    class GenerateSimulationScene
    {
    private:
        /* data */

    public:
        GenerateSimulationScene(/* args */);
        ~GenerateSimulationScene();

        /**
         * @brief The method for generating simulation scenarios
         * @param[in] simulator the state of simulator running
         * @return true return true if successful
         * @return false return false if failed
         */
        virtual bool operator()(nlohmann::ordered_json &simulator);
    };

} // namespace simu

#endif // GENERATE_SIMULATION_SCENE_H