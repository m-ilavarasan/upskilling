from transformers import AutoTokenizer, AutoModelForCausalLM
import torch

# Load Meta's Llama 2 model from Hugging Face
model_name = "meta-llama/Llama-2-7b-hf"  # or Llama-2-13b-hf for larger model
tokenizer = AutoTokenizer.from_pretrained(model_name)
model = AutoModelForCausalLM.from_pretrained(model_name, torch_dtype=torch.float16, device_map="auto")

# Generate text based on a prompt
prompt = "What is the future of artificial intelligence?"
inputs = tokenizer(prompt, return_tensors="pt").to(model.device)
outputs = model.generate(**inputs, max_length=100, num_return_sequences=1)
generated_text = tokenizer.decode(outputs[0], skip_special_tokens=True)
print("Generated Text:", generated_text)