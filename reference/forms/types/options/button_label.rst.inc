label
~~~~~

**データ型**: ``string`` **デフォルト**: ラベルはフィールド名から"推測"されます

ボタンに表示されるラベルを設定します。直接テンプレート内で設定することもできます。:

.. configuration-block::

    .. code-block:: html+jinja

        {{ form_widget(form.save, { 'label': 'Click me' }) }}

    .. code-block:: html+php

        <?php echo $view['form']->widget($form['save'], array('label' => 'Click me')) ?>

.. 2013.11.23 kang1123 88b88e5bb47b0f39a634c6efc409902db3359a57
