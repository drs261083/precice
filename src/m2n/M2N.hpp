#pragma once

#include "DistributedComFactory.hpp"

#include "com/SharedPointer.hpp"
#include "mesh/SharedPointer.hpp"
#include "logging/Logger.hpp"

#include <map>

namespace precice {
namespace m2n {

/**
 * @brief M2N communication class.
 * This layer is necessary since communication between two participants can be working via several meshes,
 * each possibly with a different decomposition. In principle, this class is only a map from meshes to DistributedCommunications
 *
 */
class M2N
{
public:

  M2N( com::PtrCommunication masterCom, DistributedComFactory::SharedPointer distrFactory);

  /**
   * @brief Destructor, empty.
   */
  ~M2N();

  /// Returns true, if a connection to a remote participant has been setup.
  bool isConnected();

  /**
   * @brief Connects to another participant, which has to call requestConnection().
   *
   * @param[in] nameAcceptor Name of calling participant.
   * @param[in] nameRequester Name of remote participant to connect to.
   */
  void acceptMasterConnection (
    const std::string& nameAcceptor,
    const std::string& nameRequester);

  /**
   * @brief Connects to another participant, which has to call acceptConnection().
   *
   * @param[in] nameAcceptor Name of remote participant to connect to.
   * @param[in] nameReuester Name of calling participant.
   */
  void requestMasterConnection (
    const std::string& nameAcceptor,
    const std::string& nameRequester);


  /**
   * @brief Connects to another participant, which has to call requestConnection().
   *
   * @param[in] nameAcceptor Name of calling participant.
   * @param[in] nameRequester Name of remote participant to connect to.
   */
  void acceptSlavesConnection (
    const std::string& nameAcceptor,
    const std::string& nameRequester);

  /**
   * @brief Connects to another participant, which has to call acceptConnection().
   *
   * @param[in] nameAcceptor Name of remote participant to connect to.
   * @param[in] nameReuester Name of calling participant.
   */
  void requestSlavesConnection (
    const std::string& nameAcceptor,
    const std::string& nameRequester);

  /**
   * @brief Disconnects from communication space, i.e. participant.
   *
   * This method is called on destruction.
   */
  void closeConnection();

  /**
   * @brief Get the basic communication between the 2 masters.
   */
  com::PtrCommunication getMasterCommunication();


  void createDistributedCommunication(mesh::PtrMesh mesh);

  void startSendPackage ( int rankReceiver );

  void finishSendPackage();

  /**
   * @brief Starts to receive messages from rankSender.
   *
   * @return Rank of sender, which is useful when ANY_SENDER is used.
   */
  int startReceivePackage ( int rankSender );

  void finishReceivePackage();


  /**
   * @brief Sends an array of double values from all slaves (different for each slave).
   */
  void send (
    double* itemsToSend,
    int     size,
    int     meshID,
    int     valueDimension );

  /**
   * @brief The master sends a bool to the other master, for performance reasons, we
   * neglect the gathering and checking step.
   */
  void send (
    bool   itemToSend);


  /**
   * @brief The master sends a double to the other master, for performance reasons, we
   * neglect the gathering and checking step.
   */
  void send (
    double itemToSend);

  /**
   * @brief All slaves receive an array of doubles (different for each slave).
   */
  void receive (
    double* itemsToReceive,
    int     size,
    int     meshID,
    int     valueDimension );

  /**
   * @brief All slaves receive a bool (the same for each slave).
   */
  void receive (
    bool&  itemToReceive);

  /**
   * @brief All slaves receive a double (the same for each slave).
   */
  void receive (
    double&  itemToReceive);

private:

  static logging::Logger _log;

  std::map<int, DistributedCommunication::SharedPointer> _distComs;

  com::PtrCommunication _masterCom;

  DistributedComFactory::SharedPointer _distrFactory;

  bool _isMasterConnected;

  bool _areSlavesConnected;


};

}} // namespace precice, m2n
