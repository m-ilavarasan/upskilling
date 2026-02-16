"""
Emotion Detection Server

This script defines a Flask-based server for performing emotion detection on user-provided text.

Author(Learner): [NoorAldin]
"""

from flask import Flask, render_template, request  # pylint: disable=import-error
from EmotionDetection.emotion_detection import emotion_detector  # pylint: disable=import-error
from EmotionDetection.emotion_detection import emotion_predictor  # pylint: disable=import-error

app = Flask("Emotion Detection")

def run_emotion_detection():
    """
    Main function to run the Emotion Detection application.
    """
    app.run(host="0.0.0.0", port=5000)

@app.route("/emotionDetector")
def sent_detector():
    """
    Analyze the user-provided text for emotions and return the result.
    """
    text_to_detect = request.args.get("textToAnalyze")
    response = emotion_detector(text_to_detect)
    formatted_response = emotion_predictor(response)
    if formatted_response.get("dominant_emotion") is None:
        return "Invalid text! Please try again."

    parts = [
        "For the given statement, the system response is ",
        "'anger': ", str(formatted_response.get("anger")), " ",
        "'disgust': ", str(formatted_response.get("disgust")), ", ",
        "'fear': ", str(formatted_response.get("fear")), ", ",
        "'joy': ", str(formatted_response.get("joy")), " ",
        "and 'sadness': ", str(formatted_response.get("sadness")), ". ",
        "The dominant emotion is ", str(formatted_response.get("dominant_emotion")), ".",
    ]

    return "".join(parts)


@app.route("/")
def render_index_page():
    ''' This function initiates the rendering of the main application
        page over the Flask channel
    '''
    return render_template('index.html')

if __name__ == "__main__":
    run_emotion_detection()