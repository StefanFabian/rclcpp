// Copyright 2016 Open Source Robotics Foundation, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef RCLCPP__NODE_INTERFACES__NODE_PARAMETERS_INTERFACE_HPP_
#define RCLCPP__NODE_INTERFACES__NODE_PARAMETERS_INTERFACE_HPP_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "rcl_interfaces/msg/list_parameters_result.hpp"
#include "rcl_interfaces/msg/parameter_descriptor.hpp"
#include "rcl_interfaces/msg/set_parameters_result.hpp"

#include "rclcpp/macros.hpp"
#include "rclcpp/parameter.hpp"
#include "rclcpp/visibility_control.hpp"

namespace rclcpp
{
namespace node_interfaces
{

struct PreSetParametersCallbackHandle
{
  RCLCPP_SMART_PTR_DEFINITIONS(PreSetParametersCallbackHandle)

  using PreSetParametersCallbackType =
    std::function<void (std::vector<rclcpp::Parameter> &)>;

  PreSetParametersCallbackType callback;
};

struct OnSetParametersCallbackHandle
{
  RCLCPP_SMART_PTR_DEFINITIONS(OnSetParametersCallbackHandle)

  using OnSetParametersCallbackType =
    std::function<
    rcl_interfaces::msg::SetParametersResult(
      const std::vector<rclcpp::Parameter> &)>;
  using OnParametersSetCallbackType [[deprecated("use OnSetParametersCallbackType instead")]] =
    OnSetParametersCallbackType;

  OnSetParametersCallbackType callback;
};

struct PostSetParametersCallbackHandle
{
  RCLCPP_SMART_PTR_DEFINITIONS(PostSetParametersCallbackHandle)

  using PostSetParametersCallbackType =
    std::function<void (const std::vector<rclcpp::Parameter> &)>;

  PostSetParametersCallbackType callback;
};

/// Pure virtual interface class for the NodeParameters part of the Node API.
class NodeParametersInterface
{
public:
  RCLCPP_SMART_PTR_ALIASES_ONLY(NodeParametersInterface)

  RCLCPP_PUBLIC
  virtual
  ~NodeParametersInterface() = default;

  /// Declare and initialize a parameter.
  /**
   * \sa rclcpp::Node::declare_parameter
   */
  RCLCPP_PUBLIC
  virtual
  const rclcpp::ParameterValue &
  declare_parameter(
    const std::string & name,
    const rclcpp::ParameterValue & default_value,
    const rcl_interfaces::msg::ParameterDescriptor & parameter_descriptor =
    rcl_interfaces::msg::ParameterDescriptor(),
    bool ignore_override = false) = 0;

  /// Declare a parameter.
  /**
   * \sa rclcpp::Node::declare_parameter
   */
  RCLCPP_PUBLIC
  virtual
  const rclcpp::ParameterValue &
  declare_parameter(
    const std::string & name,
    rclcpp::ParameterType type,
    const rcl_interfaces::msg::ParameterDescriptor & parameter_descriptor =
    rcl_interfaces::msg::ParameterDescriptor(),
    bool ignore_override = false) = 0;

  /// Undeclare a parameter.
  /**
   * \sa rclcpp::Node::undeclare_parameter
   */
  RCLCPP_PUBLIC
  virtual
  void
  undeclare_parameter(const std::string & name) = 0;

  /// Return true if the parameter has been declared, otherwise false.
  /**
   * \sa rclcpp::Node::has_parameter
   */
  RCLCPP_PUBLIC
  virtual
  bool
  has_parameter(const std::string & name) const = 0;

  /// Set one or more parameters, one at a time.
  /**
   * \sa rclcpp::Node::set_parameters
   */
  RCLCPP_PUBLIC
  virtual
  std::vector<rcl_interfaces::msg::SetParametersResult>
  set_parameters(const std::vector<rclcpp::Parameter> & parameters) = 0;

  /// Set one or more parameters, all at once.
  /**
   * \sa rclcpp::Node::set_parameters_atomically
   */
  RCLCPP_PUBLIC
  virtual
  rcl_interfaces::msg::SetParametersResult
  set_parameters_atomically(
    const std::vector<rclcpp::Parameter> & parameters) = 0;

  /// Get descriptions of parameters given their names.
  /*
   * \param[in] names a list of parameter names to check.
   * \return the list of parameters that were found.
   * Any parameter not found is omitted from the returned list.
   */
  RCLCPP_PUBLIC
  virtual
  std::vector<rclcpp::Parameter>
  get_parameters(const std::vector<std::string> & names) const = 0;

  /// Get the description of one parameter given a name.
  /*
   * \param[in] name the name of the parameter to look for.
   * \return the parameter if it exists on the node.
   * \throws std::out_of_range if the parameter does not exist on the node.
   */
  RCLCPP_PUBLIC
  virtual
  rclcpp::Parameter
  get_parameter(const std::string & name) const = 0;

  /// Get the description of one parameter given a name.
  /*
   * \param[in] name the name of the parameter to look for.
   * \param[out] parameter the description if parameter exists on the node.
   * \return true if the parameter exists on the node, or
   * \return false if the parameter does not exist.
   */
  RCLCPP_PUBLIC
  virtual
  bool
  get_parameter(
    const std::string & name,
    rclcpp::Parameter & parameter) const = 0;

  /// Get all parameters that have the specified prefix into the parameters map.
  /*
   * \param[in] prefix the name of the prefix to look for.
   * \param[out] parameters a map of parameters that matched the prefix.
   * \return true if any parameters with the prefix exists on the node, or
   * \return false otherwise.
   */
  RCLCPP_PUBLIC
  virtual
  bool
  get_parameters_by_prefix(
    const std::string & prefix,
    std::map<std::string, rclcpp::Parameter> & parameters) const = 0;

  RCLCPP_PUBLIC
  virtual
  std::vector<rcl_interfaces::msg::ParameterDescriptor>
  describe_parameters(const std::vector<std::string> & names) const = 0;

  RCLCPP_PUBLIC
  virtual
  std::vector<uint8_t>
  get_parameter_types(const std::vector<std::string> & names) const = 0;

  RCLCPP_PUBLIC
  virtual
  rcl_interfaces::msg::ListParametersResult
  list_parameters(const std::vector<std::string> & prefixes, uint64_t depth) const = 0;

  using OnSetParametersCallbackType = OnSetParametersCallbackHandle::OnSetParametersCallbackType;
  using PostSetParametersCallbackType =
    PostSetParametersCallbackHandle::PostSetParametersCallbackType;
  using PreSetParametersCallbackType = PreSetParametersCallbackHandle::PreSetParametersCallbackType;

  /// Add a callback that gets triggered before parameters are validated.
  /**
   * \sa rclcpp::Node::add_pre_set_parameters_callback
   */
  RCLCPP_PUBLIC
  virtual
  PreSetParametersCallbackHandle::SharedPtr
  add_pre_set_parameters_callback(PreSetParametersCallbackType callback) = 0;

  /// Add a callback to validate parameters before they are set.
  /**
   * \sa rclcpp::Node::add_on_set_parameters_callback
   */
  RCLCPP_PUBLIC
  virtual
  OnSetParametersCallbackHandle::SharedPtr
  add_on_set_parameters_callback(OnSetParametersCallbackType callback) = 0;

  /// Add a callback that gets triggered after parameters are set successfully.
  /**
   * \sa rclcpp::Node::add_post_set_parameters_callback
   */
  RCLCPP_PUBLIC
  virtual
  PostSetParametersCallbackHandle::SharedPtr
  add_post_set_parameters_callback(PostSetParametersCallbackType callback) = 0;

  /// Remove a callback registered with `add_pre_set_parameters_callback`.
  /**
   * \sa rclcpp::Node::remove_pre_set_parameters_callback
   */
  RCLCPP_PUBLIC
  virtual
  void
  remove_pre_set_parameters_callback(const PreSetParametersCallbackHandle * const handler) = 0;

  /// Remove a callback registered with `add_on_set_parameters_callback`.
  /**
   * \sa rclcpp::Node::remove_on_set_parameters_callback
   */
  RCLCPP_PUBLIC
  virtual
  void
  remove_on_set_parameters_callback(const OnSetParametersCallbackHandle * const handler) = 0;

  /// Remove a callback registered with `add_post_set_parameters_callback`.
  /**
   * \sa rclcpp::Node::remove_post_set_parameters_callback
   */
  RCLCPP_PUBLIC
  virtual
  void
  remove_post_set_parameters_callback(const PostSetParametersCallbackHandle * const handler) = 0;

  /// Return the initial parameter values used by the NodeParameters to override default values.
  RCLCPP_PUBLIC
  virtual
  const std::map<std::string, rclcpp::ParameterValue> &
  get_parameter_overrides() const = 0;
};

}  // namespace node_interfaces
}  // namespace rclcpp

#endif  // RCLCPP__NODE_INTERFACES__NODE_PARAMETERS_INTERFACE_HPP_
