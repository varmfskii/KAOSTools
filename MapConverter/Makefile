CXXFLAGS+=-I. -I../include
LDFLAGS+=-L../lib
LIBS=-lkaos -lpugixml -ltiled
SRC=DescriptorNode.cpp main.cpp MapConverter.cpp MapConverter_Legacy.cpp
BUILDER=Builder/AsmFormatter.cpp Builder/DataBuilder.cpp		\
	Builder/DataGenerator.cpp Builder/DataSource.cpp		\
	Builder/DefinitionBuilder.cpp Builder/MapDataSource.cpp		\
	Builder/ObjectDataSource.cpp Builder/SimpleDataBuilder.cpp	\
	Builder/TileDataSource.cpp Builder/ValueDataBuilder.cpp
DEFINITON=DefinitionNodes/Defintion.cpp DefinitionNodes/Node.cpp	\
	DefinitionNodes/SymbolicValue.cpp				\
	DefinitionNodes/Variable.cpp
DESCRIPTOR=DescriptorNodes/BitField.cpp					\
        DescriptorNodes/CompositeNode.cpp DescriptorNodes/Layer.cpp	\
        DescriptorNodes/MapFile.cpp DescriptorNodes/Object.cpp		\
        DescriptorNodes/ObjectLayer.cpp DescriptorNodes/Objects.cpp	\
        DescriptorNodes/Option.cpp DescriptorNodes/OptionTable.cpp	\
        DescriptorNodes/PackedValue.cpp					\
        DescriptorNodes/PropertyQuery.cpp DescriptorNodes/Root.cpp	\
        DescriptorNodes/TileDescriptor.cpp				\
        DescriptorNodes/TiledLayer.cpp					\
        DescriptorNodes/TilesetDescriptor.cpp				\
        DescriptorNodes/TypedNode.cpp					\
        DescriptorNodes/TypedPropertyQuery.cpp				\
        DescriptorNodes/TypedValue.cpp
SRCS=$(SRC) $(BUILDER) $(DEFINITION) $(DESCRIPTOR)
OBJS=$(SRCS:cpp=o)
TGTS=MapConverter

all: Builder $(TGTS)

MapConverter: $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

.PHONY: all Builder

