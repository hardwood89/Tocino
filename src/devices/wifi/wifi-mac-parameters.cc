/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005 INRIA
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as 
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

#include "ns3/assert.h"
#include "wifi-mac-parameters.h"
#include "wifi-default-parameters.h"

namespace ns3 {

WifiMacParameters::WifiMacParameters ()
{
  m_rtsCtsThreshold = WifiDefaultParameters::GetRtsCtsThreshold ();
  m_fragmentationThreshold = WifiDefaultParameters::GetFragmentationThreshold ();
  m_maxSsrc = WifiDefaultParameters::GetMaxSsrc ();
  m_maxSlrc = WifiDefaultParameters::GetMaxSlrc ();

  // ensure something not too stupid is set by default.
  NS_ASSERT (WifiDefaultParameters::GetPhyStandard () == WIFI_PHY_STANDARD_80211a ||
             WifiDefaultParameters::GetPhyStandard () == WIFI_PHY_STANDARD_holland);
  uint32_t ctsAckSize = (2 + 2 + 6) * 8; // bits
  double dataRate = (6e6 / 2); // mb/s
  Time delay = Seconds (ctsAckSize / dataRate);

  Initialize (delay, delay);
}
void 
WifiMacParameters::Initialize (Time ctsDelay, Time ackDelay)
{
  NS_ASSERT (WifiDefaultParameters::GetPhyStandard () == WIFI_PHY_STANDARD_80211a ||
             WifiDefaultParameters::GetPhyStandard () == WIFI_PHY_STANDARD_holland);

  // these values are really 802.11a specific
  m_sifs = MicroSeconds (16);
  m_slot = MicroSeconds (9);


  /* see section 9.2.10 ieee 802.11-1999 */
  m_pifs = m_sifs + m_slot;
  // 1000m 
  m_maxPropagationDelay = Seconds (1000.0 / 300000000.0);
  /* Cts_Timeout and Ack_Timeout are specified in the Annex C 
     (Formal description of MAC operation, see details on the 
     Trsp timer setting at page 346)
  */
  m_ctsTimeout = m_sifs;
  m_ctsTimeout += ctsDelay;
  m_ctsTimeout += m_maxPropagationDelay * Scalar (2);
  m_ctsTimeout += m_slot;

  m_ackTimeout = m_sifs;
  m_ackTimeout += ackDelay;
  m_ackTimeout += m_maxPropagationDelay * Scalar (2);
  m_ackTimeout += m_slot;
}

void 
WifiMacParameters::SetSlotTime (Time slotTime)
{
  m_slot = slotTime;
}


Time
WifiMacParameters::GetPifs (void) const
{
  return m_pifs;
}
Time
WifiMacParameters::GetSifs (void) const
{
  return m_sifs;
}
Time
WifiMacParameters::GetSlotTime (void) const
{
  return m_slot;
}
Time
WifiMacParameters::GetCtsTimeout (void) const
{
  return m_ctsTimeout;
}
Time
WifiMacParameters::GetAckTimeout (void) const
{
  return m_ackTimeout;
}

Time
WifiMacParameters::GetBeaconInterval (void) const
{
  return Seconds (1);
}
uint32_t 
WifiMacParameters::GetMaxSsrc (void) const
{
  return m_maxSsrc;
}
uint32_t 
WifiMacParameters::GetMaxSlrc (void) const
{
  return m_maxSlrc;
}
uint32_t 
WifiMacParameters::GetRtsCtsThreshold (void) const
{
  return m_rtsCtsThreshold;
}
uint32_t 
WifiMacParameters::GetFragmentationThreshold (void) const
{
  NS_ASSERT (GetMaxMsduSize () / 16 < m_fragmentationThreshold);
  return m_fragmentationThreshold;
}
Time
WifiMacParameters::GetMsduLifetime (void) const
{
  return Seconds (10);
}
Time
WifiMacParameters::GetMaxPropagationDelay (void) const
{
  return m_maxPropagationDelay;
}

uint32_t 
WifiMacParameters::GetMaxMsduSize (void) const
{
  return 2304;
}
double 
WifiMacParameters::GetCapLimit (void) const
{
  return 0.4;
}
double 
WifiMacParameters::GetMinEdcaTrafficProportion (void) const
{
  return 0.4;
}

} // namespace ns3