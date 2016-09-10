#pragma once
/*
 *      Copyright (C) 2005-2016 Team Kodi
 *      http://xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */



// tau release
#define COMPRESSOR_TAU_RELEASE_DEFAULT        1.0f
#define COMPRESSOR_TAU_RELEASE_MIN            0.1f
#define COMPRESSOR_TAU_RELEASE_MAX            30.0f

// tau attack
#define COMPRESSOR_TAU_ATTACK_DEFAULT         1.0f
#define COMPRESSOR_TAU_ATTACK_MIN             0.1f
#define COMPRESSOR_TAU_ATTACK_MAX             30.0f

// threshold
#define COMPRESSOR_THRESHOLD_DEFAULT          0.0f
#define COMPRESSOR_THRESHOLD_MIN              -120.0f
#define COMPRESSOR_THRESHOLD_MAX              120.0f

// compression ration
#define COMPRESSOR_COMPRESSION_RATIO_DEFAULT  9.0f
#define COMPRESSOR_COMPRESSION_RATIO_MIN      0.0f
#define COMPRESSOR_COMPRESSION_RATIO_MAX      120.0f

// knee width
#define COMPRESSOR_KNEE_WIDTH_DEFAULT         0.0f
#define COMPRESSOR_KNEE_WIDTH_MIN             0.0f
#define COMPRESSOR_KNEE_WIDTH_MAX             120.0f
 