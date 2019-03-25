//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#pragma once
#include <Tiled/Tile.h>
#include <Tiled/Map.h>


namespace Builder
{

	class DataSource
	{
	public:

		class ObjectDescriptor
		{
		public:

			virtual ~ObjectDescriptor() = default;
		};


	public:

		using property_type = KAOS::Tiled::PropertyBag::value_type;
		using layer_type = KAOS::Tiled::Layer;
		using objectlayer_type = KAOS::Tiled::ObjectGroupLayer;
		using objectdescriptor_type = ObjectDescriptor;


	public:

		virtual ~DataSource() = default;

		virtual std::optional<property_type> QueryProperty(const std::string& name) const = 0;
		virtual std::shared_ptr<layer_type> QueryLayer(const std::string& name) const;
		virtual std::shared_ptr<objectlayer_type> QueryObjectLayer(const std::string& name) const;
		virtual std::shared_ptr<objectdescriptor_type> QueryObjectDescriptor(const std::string& name) const;
	};


	class TileDataSource : public DataSource
	{
	public:
	
		explicit TileDataSource(const KAOS::Tiled::Tile& tile);


		std::optional<property_type> QueryProperty(const std::string& name) const override;


	private:

		const KAOS::Tiled::Tile&	m_Tile;
	};


	class ObjectDataSource : public DataSource
	{
	public:
	
		ObjectDataSource(const KAOS::Tiled::Object& object, unsigned long long typeId);


		std::optional<property_type> QueryProperty(const std::string& name) const override;


	private:

		const KAOS::Tiled::Object&	m_Object;
		const unsigned long long	m_TypeId;
	};


	class MapDataSource : public DataSource
	{
	public:
	
		explicit MapDataSource(const KAOS::Tiled::Map& map);


		std::optional<property_type> QueryProperty(const std::string& name) const override;
		std::shared_ptr<objectlayer_type> QueryObjectLayer(const std::string& name) const override;


	private:

		const KAOS::Tiled::Map&	m_Map;
	};

	template<class ObjectSourceType_>
	class MapDataSourceEx : public MapDataSource
	{
	public:

		MapDataSourceEx(const KAOS::Tiled::Map& map, ObjectSourceType_& objectSource)
			:
			MapDataSource(map),
			m_ObjectSource(objectSource)
		{}


		std::shared_ptr<objectdescriptor_type> QueryObjectDescriptor(const std::string& name) const override
		{
			return m_ObjectSource.Query(name);
		}


	protected:

		ObjectSourceType_& m_ObjectSource;
	};
}




//	Copyright (c) 2018 Chet Simpson
//	
//	Permission is hereby granted, free of charge, to any person
//	obtaining a copy of this software and associated documentation
//	files (the "Software"), to deal in the Software without
//	restriction, including without limitation the rights to use,
//	copy, modify, merge, publish, distribute, sublicense, and/or sell
//	copies of the Software, and to permit persons to whom the
//	Software is furnished to do so, subject to the following
//	conditions:
//	
//	The above copyright notice and this permission notice shall be
//	included in all copies or substantial portions of the Software.
//	
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
//	OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//	NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//	WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//	OTHER DEALINGS IN THE SOFTWARE.
