//
// Created by alex on 15.11.2022.
//

#pragma once

#include "sim/physics/force/FLennardJonesOMP.h"
#include "sim/physics/position/XStoermerVelvetOMP.h"
#include "sim/physics/velocity/VStoermerVelvetOMP.h"

#include <limits>

constexpr double default_delta_t{1.0};
constexpr double default_end_time{5.0};
constexpr double default_start_time{0};
constexpr auto default_output_base_name{"result"};
constexpr auto default_output_folder{"./output/"};
constexpr double default_epsilon{1};
constexpr double default_sigma{1};
constexpr double default_brown{0.1};
constexpr int default_dims{3};
constexpr int default_bench_iterations{10};
constexpr int default_bench_maxBody{2};
constexpr auto default_boundary_cond_str {"outflow"}; //this must be kept lowercase, or else fun :D
constexpr auto default_force_type {"lennardjones"};
constexpr auto default_pos_type {"stoermervelvet"};
constexpr auto default_vel_type {"stoermervelvet"};
constexpr bool default_linked_cell{true};
constexpr double default_bound_x0{100.0};
constexpr double default_bound_x1{100.0};
constexpr double default_bound_x2{100.0};
constexpr double default_r_cutoff{5.0};
constexpr int default_log_level{2};
constexpr int default_therm{1}; // either 1 or 0
constexpr double default_t_init{20.0};
constexpr int default_n_term{0};
constexpr double default_t_target{20.0};
constexpr double default_delta_temp{std::numeric_limits<double>::infinity()}; // DO NOT CHANGE THIS!
constexpr bool default_checkpointing{true};
constexpr double default_g_grav{1.0};
constexpr auto default_loader{"xml"};
constexpr int default_last_iteration{0};

using calcF = sim::physics::force::FLennardJonesOMP;
using calcX = sim::physics::position::XStoermerVelvetOMP;
using calcV = sim::physics::velocity::VStoermerVelvetOMP;
