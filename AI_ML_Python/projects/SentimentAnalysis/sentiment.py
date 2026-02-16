"""
This module contains functions to perform sentiment analysis
using the Watson AI sentiment analysis API.
"""

import json
import requests


def sentiment_analyzer(text_to_analyse):
    """
    Sends the input text to the Watson sentiment analysis API and returns the response.

    Args:
        text_to_analyse (str): The text string to analyze sentiment for.

    Returns:
        dict: A dictionary with keys 'label' and 'score'. Values are None when
        unavailable or when an error occurs.
    """
    label = None
    score = None

    # Define the URL for the sentiment analysis API
    url = (
        "https://sn-watson-sentiment-bert.labs.skills.network/"
        "v1/watson.runtime.nlp.v1/NlpService/SentimentPredict"
    )
    # Create the payload with the text to be analyzed
    myobj = {
        "raw_document": {
            "text": text_to_analyse
        }
    }
    # Set the headers with the required model ID for the API
    header = {
        "grpc-metadata-mm-model-id": "sentiment_aggregated-bert-workflow_lang_multi_stock"
    }

    try:
        # Make a POST request to the API with the payload and headers
        response = requests.post(url, json=myobj, headers=header, timeout=10)
    except requests.RequestException:
        # Network or connection error; return None values
        return {"label": None, "score": None}

    try:
        # Parse the response from the API
        formatted_response = json.loads(response.text)
    except ValueError:
        return {"label": None, "score": None}

    # If the response status code is 200, extract the label and score from the response
    if response.status_code == 200:
        document_sentiment = formatted_response.get("documentSentiment") or {}
        label = document_sentiment.get("label")
        score = document_sentiment.get("score")
    else:
        # For non-200 responses, return None values
        label = None
        score = None

    # Return the label and score in a dictionary
    return {"label": label, "score": score}
