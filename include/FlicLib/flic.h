// Aseprite FLIC Library
// Copyright (c) 2015 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#ifndef FLIC_FLIC_H_INCLUDED
#define FLIC_FLIC_H_INCLUDED
#pragma once

#include <stdint.h>

#include <cassert>
#include <cstdio>
#include <vector>
#include <istream>

namespace flic {

  struct Color {
    uint8_t r, g, b;

    Color() : r(0), g(0), b(0) {
    }

    Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {
    }

    bool operator==(const Color& o) const {
      return r == o.r && g == o.g && b == o.b;
    }

    bool operator!=(const Color& o) const {
      return !operator==(o);
    }
  };

  struct Header {
    int frames;
    int width;
    int height;
    int speed;
  };

  class Colormap {
  public:
    static const int SIZE = 256;

    Colormap() {
    }

    int size() const {
      return SIZE;
    }

    const Color& operator[](int i) const {
      assert(i >= 0 && i < SIZE);
      return m_color[i];
    }

    Color& operator[](int i) {
      assert(i >= 0 && i < SIZE);
      return m_color[i];
    }

    bool operator==(const Colormap& o) const {
      for (int i=0; i<SIZE; ++i) {
        if (m_color[i] != o[i])
          return false;
      }
      return true;
    }

    bool operator!=(const Colormap& o) const {
      return !operator==(o);
    }

  private:
    Color m_color[SIZE];
  };

  struct Frame
  {
	  Frame(unsigned int width, unsigned int height, unsigned int rowstride)
		  :
		  width(width),
		  height(height),
		  rowstride(rowstride),
		  pixels(new uint8_t[rowstride * height])
	  {}

	  ~Frame()
	  {
		  delete[] pixels;
	  }


	  uint8_t* const	pixels;
	  const uint32_t	width;
	  const uint32_t	height;
	  const uint32_t	rowstride;
	  Colormap			colormap;
  };

  class FileInputInterface {
  public:
	virtual ~FileInputInterface() = default;

    // Returns true if we can read/write bytes from/into the file
    virtual bool ok() const = 0;

    // Current position in the file
    virtual size_t tell() = 0;

    // Jump to the given position in the file
    virtual void seek(size_t absPos) = 0;

    // Returns the next byte in the file or 0 if ok() = false
    virtual uint8_t read8() = 0;
  };


  class FileInterface : public FileInputInterface {
  public:

	  // Writes one byte in the file (or do nothing if ok() = false)
	  virtual void write8(uint8_t value) = 0;
  };




  class StdioFileInterface : public flic::FileInterface {
  public:
    StdioFileInterface(FILE* file);
    bool ok() const override;
    size_t tell() override;
    void seek(size_t absPos) override;
    uint8_t read8() override;
    void write8(uint8_t value) override;

  private:
    FILE* m_file;
    bool m_ok;
  };


  class StreamInputInterface : public FileInputInterface
  {
  public:
	  StreamInputInterface(std::istream& input);

	  bool ok() const override;
	  size_t tell() override;
	  void seek(size_t absPos) override;
	  uint8_t read8() override;


  private:
	  std::istream&	m_Input;
	  bool			m_Ok;
  };


  class Decoder {
  public:
    Decoder(FileInputInterface* file);
    bool readHeader(Header& header);
    bool readFrame(Frame& frame);

  private:
    void readChunk(Frame& frame);
    void readBlackChunk(Frame& frame);
    void readCopyChunk(Frame& frame);
    void readColorChunk(Frame& frame, bool oldColorChunk);
    void readBrunChunk(Frame& frame);
    void readLcChunk(Frame& frame);
    void readDeltaChunk(Frame& frame);
    uint16_t read16();
    uint32_t read32();

	FileInputInterface* m_file;
    int m_width, m_height;
    int m_frameCount;
    int m_offsetFrame1;
    int m_offsetFrame2;
  };

  class Encoder {
  public:
    Encoder(FileInterface* file);
    ~Encoder();

    void writeHeader(const Header& header);
    void writeFrame(const Frame& frame);

    // Must be called at the end with the first frame. It's required
    // by Animator Pro to loop the animation from the last frame to
    // the first one.
    void writeRingFrame(const Frame& frame);

  private:
    void writeColorChunk(const Frame& frame);
    void writeBrunChunk(const Frame& frame);
    void writeBrunLineChunk(const Frame& frame, int y);
    void writeLcChunk(const Frame& frame);
    void writeLcLineChunk(const Frame& frame, int y);
    void write16(uint16_t value);
    void write32(uint32_t value);

    FileInterface* m_file;
    int m_width, m_height;
    Colormap m_prevColormap;
    std::vector<uint8_t> m_prevFrameData;
    int m_frameCount;
    int m_offsetFrame1;
    int m_offsetFrame2;
  };

} // namespace flic

#endif
