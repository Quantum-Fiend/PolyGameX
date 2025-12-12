package com.polygamex.assets;

import com.fasterxml.jackson.databind.ObjectMapper;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.*;
import java.nio.file.*;
import java.util.*;

/**
 * Asset processing and optimization utilities.
 * 
 * Handles:
 * - Asset import and conversion
 * - Texture compression
 * - Model optimization
 * - Asset metadata management
 */
public class AssetProcessor {
    private static final Logger logger = LoggerFactory.getLogger(AssetProcessor.class);
    private final ObjectMapper objectMapper;
    private final Path assetDirectory;
    
    public AssetProcessor(String assetDirectoryPath) {
        this.assetDirectory = Paths.get(assetDirectoryPath);
        this.objectMapper = new ObjectMapper();
        
        try {
            Files.createDirectories(assetDirectory);
        } catch (IOException e) {
            logger.error("Failed to create asset directory", e);
        }
    }
    
    /**
     * Process and import an asset file.
     */
    public boolean processAsset(String sourcePath, AssetType type) {
        logger.info("Processing asset: {} (type: {})", sourcePath, type);
        
        try {
            Path source = Paths.get(sourcePath);
            if (!Files.exists(source)) {
                logger.error("Source file not found: {}", sourcePath);
                return false;
            }
            
            switch (type) {
                case TEXTURE:
                    return processTexture(source);
                case MODEL:
                    return processModel(source);
                case AUDIO:
                    return processAudio(source);
                case SCRIPT:
                    return processScript(source);
                default:
                    logger.warn("Unknown asset type: {}", type);
                    return false;
            }
        } catch (Exception e) {
            logger.error("Failed to process asset", e);
            return false;
        }
    }
    
    private boolean processTexture(Path source) throws IOException {
        logger.info("Processing texture: {}", source.getFileName());
        
        // Create asset metadata
        AssetMetadata metadata = new AssetMetadata();
        metadata.setName(source.getFileName().toString());
        metadata.setType(AssetType.TEXTURE);
        metadata.setSourcePath(source.toString());
        metadata.setImportDate(new Date());
        
        // Copy to asset directory
        Path destination = assetDirectory.resolve("textures").resolve(source.getFileName());
        Files.createDirectories(destination.getParent());
        Files.copy(source, destination, StandardCopyOption.REPLACE_EXISTING);
        
        // Save metadata
        saveMetadata(metadata, destination.toString() + ".meta");
        
        logger.info("Texture processed successfully");
        return true;
    }
    
    private boolean processModel(Path source) throws IOException {
        logger.info("Processing model: {}", source.getFileName());
        
        AssetMetadata metadata = new AssetMetadata();
        metadata.setName(source.getFileName().toString());
        metadata.setType(AssetType.MODEL);
        metadata.setSourcePath(source.toString());
        metadata.setImportDate(new Date());
        
        Path destination = assetDirectory.resolve("models").resolve(source.getFileName());
        Files.createDirectories(destination.getParent());
        Files.copy(source, destination, StandardCopyOption.REPLACE_EXISTING);
        
        saveMetadata(metadata, destination.toString() + ".meta");
        
        logger.info("Model processed successfully");
        return true;
    }
    
    private boolean processAudio(Path source) throws IOException {
        logger.info("Processing audio: {}", source.getFileName());
        
        AssetMetadata metadata = new AssetMetadata();
        metadata.setName(source.getFileName().toString());
        metadata.setType(AssetType.AUDIO);
        metadata.setSourcePath(source.toString());
        metadata.setImportDate(new Date());
        
        Path destination = assetDirectory.resolve("audio").resolve(source.getFileName());
        Files.createDirectories(destination.getParent());
        Files.copy(source, destination, StandardCopyOption.REPLACE_EXISTING);
        
        saveMetadata(metadata, destination.toString() + ".meta");
        
        logger.info("Audio processed successfully");
        return true;
    }
    
    private boolean processScript(Path source) throws IOException {
        logger.info("Processing script: {}", source.getFileName());
        
        AssetMetadata metadata = new AssetMetadata();
        metadata.setName(source.getFileName().toString());
        metadata.setType(AssetType.SCRIPT);
        metadata.setSourcePath(source.toString());
        metadata.setImportDate(new Date());
        
        Path destination = assetDirectory.resolve("scripts").resolve(source.getFileName());
        Files.createDirectories(destination.getParent());
        Files.copy(source, destination, StandardCopyOption.REPLACE_EXISTING);
        
        saveMetadata(metadata, destination.toString() + ".meta");
        
        logger.info("Script processed successfully");
        return true;
    }
    
    /**
     * Get list of all processed assets.
     */
    public List<AssetMetadata> getAllAssets() {
        List<AssetMetadata> assets = new ArrayList<>();
        
        try {
            Files.walk(assetDirectory)
                .filter(path -> path.toString().endsWith(".meta"))
                .forEach(path -> {
                    try {
                        AssetMetadata metadata = loadMetadata(path.toString());
                        if (metadata != null) {
                            assets.add(metadata);
                        }
                    } catch (IOException e) {
                        logger.error("Failed to load metadata: {}", path, e);
                    }
                });
        } catch (IOException e) {
            logger.error("Failed to scan asset directory", e);
        }
        
        return assets;
    }
    
    private void saveMetadata(AssetMetadata metadata, String path) throws IOException {
        objectMapper.writerWithDefaultPrettyPrinter().writeValue(new File(path), metadata);
    }
    
    private AssetMetadata loadMetadata(String path) throws IOException {
        return objectMapper.readValue(new File(path), AssetMetadata.class);
    }
    
    /**
     * Asset types supported by the engine.
     */
    public enum AssetType {
        TEXTURE,
        MODEL,
        AUDIO,
        SCRIPT,
        SCENE,
        MATERIAL
    }
    
    /**
     * Asset metadata class.
     */
    public static class AssetMetadata {
        private String name;
        private AssetType type;
        private String sourcePath;
        private Date importDate;
        private Map<String, Object> properties = new HashMap<>();
        
        // Getters and setters
        public String getName() { return name; }
        public void setName(String name) { this.name = name; }
        
        public AssetType getType() { return type; }
        public void setType(AssetType type) { this.type = type; }
        
        public String getSourcePath() { return sourcePath; }
        public void setSourcePath(String sourcePath) { this.sourcePath = sourcePath; }
        
        public Date getImportDate() { return importDate; }
        public void setImportDate(Date importDate) { this.importDate = importDate; }
        
        public Map<String, Object> getProperties() { return properties; }
        public void setProperties(Map<String, Object> properties) { this.properties = properties; }
    }
}
