import copy
import KernelParameters

################################################################################
# Non-Tile Parameters
################################################################################
precisions = ["s", "d", "c", "z"]

orders = [ "clblasColumnMajor" ]

transposes = { "s":["N", "T"], "d":["N", "T"], \
    "c":["N", "T", "C"], "z":["N", "T", "C"] }

unrolls = { "s":[16, 8, 1], "d":[8, 1], "c":[8, 1], "z":[8, 1] }

betas = [ 0, 1 ]

def getTilesForPrecision(precision):
  # valid tiles for this precision
  tiles = []
  tile = KernelParameters.TileParameters()
  for sizeData in kernelSelectionData[precision]:
    fallbackTile = sizeData[1]
    validTiles = sizeData[2]
    # add valid tiles
    for tileParams in validTiles:
      #print(tileParams)
      tile.workGroupNumRows = tileParams[0]
      tile.workGroupNumCols = tileParams[1]
      tile.microTileNumRows = tileParams[2]
      tile.microTileNumCols = tileParams[3]
      tile.macroTileNumRows = tile.workGroupNumRows*tile.microTileNumRows
      tile.macroTileNumCols = tile.workGroupNumCols*tile.microTileNumCols
      #print(tile.getName())
      for unroll in unrolls[precision]:
        tile.unroll = unroll
        if tile.isValid():
          tiles.append( copy.copy(tile) )
        else:
          print(tile.getName() + " - SKIPPING - ")

    # add fallback tile
    tile.workGroupNumRows = fallbackTile[0]
    tile.workGroupNumCols = fallbackTile[1]
    tile.microTileNumRows = fallbackTile[2]
    tile.microTileNumCols = fallbackTile[3]
    tile.macroTileNumRows = tile.workGroupNumRows*tile.microTileNumRows
    tile.macroTileNumCols = tile.workGroupNumCols*tile.microTileNumCols
    for unroll in unrolls[precision]:
      tile.unroll = unroll
      if tile.isValid():
        tiles.append( copy.copy(tile) )
      else:
        print(tile.getName() + " - SKIPPING - ")

  setTiles = set(tiles)
  tiles = list( setTiles )
  tiles.sort()
  return tiles

def getTransposeChoices():
  singleTransposes = []
  for precision in precisions:
    for transpose in transposes[precision]:
      singleTransposes.append( transpose )
  singleTransposeSet = set(singleTransposes)
  singleTranspose =  list( singleTransposeSet)
  transposeChoices = []
  for transA in singleTranspose:
    for transB in singleTranspose:
      transposePair = transA+transB
      if transposePair not in transposeChoices:
        transposeChoices.append(transposePair)
  return transposeChoices

def getTileChoices():
  tileChoices = []
  for precision in precisions:
    tilesForPrecision = getTilesForPrecision(precision)
    for t in tilesForPrecision:
      tile = str(t.workGroupNumRows*t.microTileNumRows) + "x" + str(t.workGroupNumCols*t.microTileNumCols)
      if tile not in tileChoices:
        tileChoices.append(tile)
  return tileChoices
